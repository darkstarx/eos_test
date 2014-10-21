#include <MainWindow.hpp>
#include <Application.hpp>
#include <graphics/Renderer.hpp>
#include <utils/log.hpp>
#include <chrono>


SDL_Window *MainWindow::window = 0;
const float MainWindow::TICK_INTERVAL = 1000.0f / 60.0f;
std::atomic<bool> MainWindow::render_working;
std::shared_ptr<std::thread> MainWindow::render_thread;


void MainWindow::init_SDL_graphics()
{
	// Инициализируем SDL графику
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG(FATAL) << "Unable to init SDL, error: " << SDL_GetError();
	}
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
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
		LOG(FATAL) << "Unable to create window, error: " << SDL_GetError();
	}
}


void MainWindow::graphics_worker()
{
	// Создаем GL-контекст
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	// Проверяем версию OpenGL
	LOG(INFO) << "OpenGL " << glGetString(GL_VERSION);
#ifdef _WIN32
	// Инициализируем GLEW
	glewExperimental = GL_TRUE;
	GLenum status = glewInit();
	if (status != GLEW_OK)
		LOG(FATAL) << "GLEW Error: " << glewGetErrorString(status);
#endif
	// Устанавливаем GL-контекст текущим
	SDL_GL_MakeCurrent(window, glcontext);
	// Создаем рендерер
	renderer_create();
	// Сообщаем рендереру о том, что поверхность сформирована
	renderer().on_surface_changed(WINDOW_WIDTH, WINDOW_HEIGHT, true);
	
	// Для контроля fps определяем максимальный fps = 60
	const std::chrono::microseconds normal_duration(static_cast<int>(TICK_INTERVAL * 1000));
	std::chrono::time_point<std::chrono::system_clock> start, end;
#ifndef NDEBUG
	int frames = 0;
	std::chrono::time_point<std::chrono::system_clock> check_point = std::chrono::system_clock::now();
#endif
	// Выполняем графический цикл, пока приложение не будет остановлено
	while (render_working)
	{
		start = std::chrono::system_clock::now();
		// Рисуем
		if (!renderer().is_valid())
		{
			renderer().on_draw_frame();
			SDL_GL_SwapWindow(window);
		}
		end = std::chrono::system_clock::now();
		// Отмечаем длительность отрисовки кадра
		const std::chrono::microseconds duration(std::chrono::duration_cast<std::chrono::microseconds>(end - start));
		// Если на отрисовку ушло меньше normal_duration, ждем следующего тика
		if (duration < normal_duration)
			std::this_thread::sleep_for(normal_duration - duration);
#ifndef NDEBUG
		++frames;
		const std::chrono::milliseconds check_time(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - check_point));
		if (check_time.count() >= 1000) {
			DLOG(INFO) << "fps: " << frames;
			frames = 0;
			check_point = std::chrono::system_clock::now();
		}
#endif
	}
	// Разрушаем рендерер
	renderer_destroy();
	// Графический поток останавливается, освобождаем GL-контекст
	SDL_GL_DeleteContext(glcontext);
}


void MainWindow::start_rendering()
{
	if (render_working) return;
	render_working = true;
	render_thread.reset(new std::thread(std::bind(&graphics_worker)));
}


void MainWindow::stop_rendering()
{
	if (!render_working) return;
	render_working = false;
	render_thread->join();
	render_thread.reset();
}


int MainWindow::create_window()
{
	// Создаем приложение
	app_create();
	
	// Создаем графическое окно
	init_SDL_graphics();
	app().start();
	
	// Время последнего тика
	std::chrono::time_point<std::chrono::system_clock> last_tick_time = std::chrono::system_clock::now();
	// Запускаем цикл обработки сообщений от окна
	bool running = true;
	while (running)
	{
		// Ожидаем сообщений (пока их нет, основной поток спит или тикает)
		SDL_Event event;
		if (SDL_WaitEventTimeout(&event, (int)TICK_INTERVAL)) {
			// Получили сообщение
			switch (event.type)
			{
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running = false;
							break;
					}
					break;
				case SDL_WINDOWEVENT:
					/** NOTE В Linux сигнал сворачивания окна приходит вместе с сигналом скрытия окна.
					 * В Linux сигнал восстановления окна приходит вместе с сигналом показа окна.
					 */
					switch (event.window.event)
					{
						case SDL_WINDOWEVENT_HIDDEN:
						case SDL_WINDOWEVENT_MINIMIZED:
							app().pause();
							stop_rendering();
							break;
						case SDL_WINDOWEVENT_SHOWN:
						case SDL_WINDOWEVENT_RESTORED:
							start_rendering();
							app().resume();
							break;
					}
					break;
			}
			std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
			std::chrono::duration<float, std::milli> duration(now - last_tick_time);
			if (duration.count() >= TICK_INTERVAL) {
				// Пора тикать
				last_tick_time = now;
				app().tick();
			}
		}
		else {
			// Не дождались сообщения, значит пришло время тикать
			last_tick_time = std::chrono::system_clock::now();
			app().tick();
		}
	}
	
	// Приложение останавливается
	app().pause();
	app().stop();
	
	// Останавливаем графический поток и разрушаем рендерер
	if (render_working) stop_rendering();
	
	// Разрушаем приложение
	app_destroy();
	
	// Деинициализируем SDL
	SDL_Quit();
	
	// Завершаем основной поток процесса
	return 0;
}
