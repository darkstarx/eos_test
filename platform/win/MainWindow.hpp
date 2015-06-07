#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <Windows.h>
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
	static const DWORD TICK_INTERVAL;						///< ����� ����� ������ � �������������

	static HINSTANCE m_hinst;								///< ��������� ����������
	static std::string m_title;								///< ��������� ����
	static std::string m_class_name;						///< ��� ������ ����
	static HWND m_window;									///< ���� ����������
	static std::atomic<bool> m_render_working;				///< ������� ���������� ������������ ������
	static std::shared_ptr<std::thread> m_render_thread;	///< ������ ���������� ����������� �������
	
	static void init_graphics();
	
	static void deinit_graphics();

	static LRESULT CALLBACK event_handler(HWND handle, UINT message, WPARAM wparam, LPARAM lparam);

	static void graphics_worker();
	
	static void start_rendering();
	
	static void stop_rendering();
	
public:
	/** \brief ������� ������� ���� ����������
	 */
	static int create_window(HINSTANCE hinst);
};

#endif // MAINWINDOW_HPP