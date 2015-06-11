#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <SDL2/SDL.h>
#include <memory>
#include <thread>
#include <atomic>

#define WINDOW_WIDTH 1196
#define WINDOW_HEIGHT 720


/** \brief ����������� ����� �������� ���� ����������
 */
class MainWindow
{
private:
	static const long int TICK_INTERVAL;					///< ����� ����� ������ � �������������
	static SDL_Window *m_window;							///< ���� ����������
	static std::atomic<bool> m_render_working;				///< ������� ���������� ������������ ������
	static std::shared_ptr<std::thread> m_render_thread;	///< ������ ���������� ����������� �������
	
	static void init_graphics();
	
	static void graphics_worker();
	
	static void start_rendering();
	
	static void stop_rendering();
	
public:
	/** \brief ������� ������� ���� ����������
	 */
	static int create_window();
};

#endif // MAINWINDOW_HPP