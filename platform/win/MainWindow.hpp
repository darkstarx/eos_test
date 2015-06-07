#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <Windows.h>
#include <memory>
#include <thread>
#include <atomic>

#define WINDOW_WIDTH 1196
#define WINDOW_HEIGHT 720


/** \brief Статический класс главного окна приложения
 */
class MainWindow
{
private:
	static const DWORD TICK_INTERVAL;						///< Время между тиками в миллисекундах

	static HINSTANCE m_hinst;								///< Экземпляр приложения
	static std::string m_title;								///< Заголовок окна
	static std::string m_class_name;						///< Имя класса окна
	static HWND m_window;									///< Окно приложения
	static std::atomic<bool> m_render_working;				///< Признак активности графического потока
	static std::shared_ptr<std::thread> m_render_thread;	///< Объект управления графическим потоком
	
	static void init_graphics();
	
	static void deinit_graphics();

	static LRESULT CALLBACK event_handler(HWND handle, UINT message, WPARAM wparam, LPARAM lparam);

	static void graphics_worker();
	
	static void start_rendering();
	
	static void stop_rendering();
	
public:
	/** \brief Создать главное окно приложения
	 */
	static int create_window(HINSTANCE hinst);
};

#endif // MAINWINDOW_HPP