#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <SDL2/SDL.h>
#include <memory>
#include <thread>
#include <atomic>

#define WINDOW_WIDTH 1196
#define WINDOW_HEIGHT 720


/** @brief ����������� ����� �������� ���� ����������
 */
class MainWindow
{
private:
	static SDL_Window * window;							///< ���� ����������
	static const float TICK_INTERVAL;					///< ����� ����� ������ � �������������
	static std::atomic<bool> render_working;			///< ������� ���������� ������������ ������
	static std::shared_ptr<std::thread> render_thread;	///< ������ ���������� ����������� �������

	static void init_SDL_graphics();

	static void graphics_worker();

	static void start_rendering();

	static void stop_rendering();
	
	static void pause();
	
	static void resume();

public:
	/** @brief ������� ������� ���� ����������
	 */
	static int create_window();
};

#endif // MAINWINDOW_HPP