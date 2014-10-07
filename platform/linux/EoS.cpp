#include <SDL2/SDL.h>
#include <utils/log.hpp>
#include <Application.hpp>
#include <thread>
#include <atomic>
#include <chrono>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


SDL_Window * window;								///< Окно приложения
static const float TICK_INTERVAL = 1000.0f / 60.0f;	///< Время между тиками в миллисекундах
std::atomic<bool> render_working;					///< признак активности графического потока
std::shared_ptr<std::thread> render_thread;			///< объект управления графическим потоком


void init_SDL_graphics()
{
	// Инициализируем SDL графику
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG(ERR) << "Unable to init SDL, error: " << SDL_GetError();
		exit(EXIT_FAILURE);
	}
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	
	static const std::string app_name = "eos_test";
	window = SDL_CreateWindow(
		app_name.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
	);
	
	if (window == NULL)
	{
		LOG(ERR) << "Unable to create window, error: " << SDL_GetError();
		exit(EXIT_FAILURE);
	}
}


void graphics_worker()
{
	// Создаем GL-контекст
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	
	// Сообщаем рендереру о том, что поверхность сформирована
	app()->renderer()->on_surface_changed(WINDOW_WIDTH, WINDOW_HEIGHT, true);
	
	// Для контроля fps определяем максимальный fps = 60
	const std::chrono::duration<float, std::milli> normal_duration(TICK_INTERVAL);
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	// Выполняем графический цикл, пока приложение не будет остановлено
	while (render_working)
	{
		start = std::chrono::steady_clock::now();
		// Рисуем
		if (!app()->renderer()->is_valid()) {
			app()->renderer()->on_draw_frame();
			SDL_GL_SwapWindow(window);
		}
		end = std::chrono::steady_clock::now();
		// Отмечаем длительность отрисовки кадра
		const std::chrono::duration<float, std::milli> duration = end - start;
		// Если на отрисовку ушло меньше normal_duration, ждем следующего тика
		if (duration < normal_duration) {
			std::this_thread::sleep_for(normal_duration - duration);
		}
	}
	// Графический поток останавливается, освобождаем GL-контекст
	SDL_GL_DeleteContext(glcontext);
}


int main(int argc, char ** argv)
{
	// Создаем приложение
	app()->create();
	
	// Создаем графическое окно
	init_SDL_graphics();
	
	// Создаем рендерер и запускаем графическую нить
	app()->create_renderer();
	render_working = true;
	render_thread.reset(new std::thread(std::bind(&graphics_worker)));
	
	// Время последнего тика
	std::chrono::time_point<std::chrono::steady_clock> last_tick_time = std::chrono::steady_clock::now();
	// Запускаем цикл обработки сообщений от окна
	bool running = true;
	while (running)
	{
		// Ожидаем сообщений (пока их нет, основной поток спит или тикает)
		SDL_Event event;
		if (SDL_WaitEventTimeout(&event, (int)TICK_INTERVAL)) {
			// Получили сообщение
			switch(event.type)
			{
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running = false;
							break;
					}
					break;
			}
			std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
			std::chrono::duration<float, std::milli> duration(now - last_tick_time);
			if (duration.count() >= TICK_INTERVAL) {
				// Пора тикать
				last_tick_time = now;
				app()->tick();
			}
		} else {
			// Не дождались сообщения, значит пришло время тикать
			last_tick_time = std::chrono::steady_clock::now();
			app()->tick();
		}
	}
	
	// Разрушаем рендерер и останавливаем графический поток
	app()->destroy_renderer();
	render_working = false;
	render_thread->join();
	
	// Разрушаем приложение
	app()->destroy();
	
	// Деинициализируем SDL
	SDL_Quit();
	
	// Завершаем основной поток процесса
	return EXIT_SUCCESS;
}
