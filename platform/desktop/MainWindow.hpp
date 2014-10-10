#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <SDL2/SDL.h>
#include <memory>
#include <thread>
#include <atomic>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


/** @brief Статический класс главного окна приложения
 */
class MainWindow
{
private:
	static SDL_Window * window;							///< Окно приложения
	static const float TICK_INTERVAL;					///< Время между тиками в миллисекундах
	static std::atomic<bool> render_working;			///< признак активности графического потока
	static std::shared_ptr<std::thread> render_thread;	///< объект управления графическим потоком

	static void init_SDL_graphics();

	static void graphics_worker();

	static void start_rendering();

	static void stop_rendering();
	
	static void pause();
	
	static void resume();

public:
	/** @brief Создать главное окно приложения
	 */
	static int create_window();
};

#endif // MAINWINDOW_HPP