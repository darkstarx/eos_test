#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <android/native_window.h>
#include <graphics/gl.hpp>
#include <atomic>
#include <memory>
#include <thread>


/** \brief Статический класс главного окна приложения
 */
class MainWindow
{
private:
	static const float TICK_INTERVAL;						///< Время между тиками в миллисекундах
	
public:
	static void set_window(EGLNativeWindowType window);
	
	static EGLNativeWindowType window() { return m_window; }
	
	static void start_rendering();
	
	static void stop_rendering();
	
private:
	static EGLNativeWindowType m_window;					///< Окно android, поддерживается в NDK, начиная с верси r5
	static EGLint m_width;									///< Ширина окна
	static EGLint m_height;									///< Высота окна
	static EGLDisplay m_display;							///< Дисплей (и так понятно какой и для чего)
	static EGLSurface m_surface;							///< Поверхность отрисовки
	static EGLContext m_context;							///< gl-контекст
	static std::atomic<bool> m_render_working;				///< Признак активности графического потока
	static std::shared_ptr<std::thread> m_render_thread;	///< Объект управления графическим потоком
	
	static void graphics_worker();
	
	static void init_graphics();
	
	static void release_graphics();
};

#endif // MAINWINDOW_H
