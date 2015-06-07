#include <MainWindow.hpp>
#include <Application.hpp>
#include <graphics/Renderer.hpp>
#include <utils/log.hpp>
#include <utils/task_queue.hpp>
#include <chrono>


const long int MainWindow::TICK_INTERVAL = 1000 / 60;
EGLNativeWindowType MainWindow::m_window = 0;
EGLint MainWindow::m_width = 0;
EGLint MainWindow::m_height = 0;
EGLDisplay MainWindow::m_display = 0;
EGLSurface MainWindow::m_surface = 0;
EGLContext MainWindow::m_context = 0;
std::atomic<bool> MainWindow::m_render_working;
std::shared_ptr<std::thread> MainWindow::m_render_thread;


void MainWindow::set_window(ANativeWindow* window)
{
	m_window = window;
}


void MainWindow::start_rendering()
{
	if (m_render_working) return;
	m_render_working = true;
	m_render_thread.reset(new std::thread(std::bind(&MainWindow::graphics_worker)));
}


void MainWindow::stop_rendering()
{
	if (!m_render_working) return;
	m_render_working = false;
	m_render_thread->join();
	m_render_thread.reset();
}


void MainWindow::graphics_worker()
{
	// Инициализируем графику
	init_graphics();
	// Оповещаем рендерер о создании GL-контекста
	renderer().on_ctx_create();
	// Сообщаем рендереру о том, что поверхность сформирована
	renderer().on_surface_changed(m_width, m_height, true);
	
	// Для контроля fps определяем максимальный fps = 60
	const std::chrono::milliseconds normal_duration(TICK_INTERVAL);
	std::chrono::time_point<std::chrono::system_clock> start, end;
#ifndef NDEBUG
	int frames = 0;
	std::chrono::time_point<std::chrono::system_clock> check_point = std::chrono::system_clock::now();
#endif
	// Выполняем графический цикл, пока приложение не будет остановлено
	while (m_render_working)
	{
		start = std::chrono::system_clock::now();
		// Обработка очереди графических задач
		renderer().tasks()->process_queue();
		// Отрисовка сцены
		if (!renderer().is_valid())
		{
			renderer().on_draw_frame();
			if (!eglSwapBuffers(m_display, m_surface))
				LOG(ERR) << "eglSwapBuffers() returned error " << eglGetError();
#ifndef NDEBUG
		++frames;
#endif
		}
		end = std::chrono::system_clock::now();
		// Отмечаем длительность отрисовки кадра
		const std::chrono::milliseconds duration(std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
		// Если на отрисовку ушло меньше normal_duration, ждем следующего тика
		if (duration < normal_duration)
			std::this_thread::sleep_for(normal_duration - duration);
#ifndef NDEBUG
		const std::chrono::milliseconds check_time(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - check_point));
		if (check_time.count() >= 1000) {
			DLOG(INFO) << "fps: " << frames;
			frames = 0;
			check_point = std::chrono::system_clock::now();
		}
#endif
	}
	// Оповещаем рендерер о разрушении GL-контекста
	renderer().on_ctx_destroy();
	// Графический поток останавливается, освобождаем GL-контекст
	release_graphics();
}


void MainWindow::init_graphics()
{
	const EGLint attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};
	EGLDisplay display;
	EGLConfig config;
	EGLint numConfigs;
	EGLint format;
	EGLSurface surface;
	EGLContext context;
	EGLint width;
	EGLint height;
	
	LOG(INFO) << "Initializing context";
	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (display == EGL_NO_DISPLAY)
		LOG(FATAL) <<  "eglGetDisplay() returned error 0x" << std::hex << eglGetError();
	
	if (!eglInitialize(display, 0, 0))
		LOG(FATAL) <<  "eglInitialize() returned error 0x" << std::hex << eglGetError();
	
	if (!eglChooseConfig(display, attribs, &config, 1, &numConfigs))
	{
		LOG(ERR) << "eglChooseConfig() returned error 0x" << std::hex << eglGetError();
		release_graphics();
		LOG(FATAL) << "Couldn't initialize graphics";
	}

	if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format))
	{
		LOG(ERR) << "eglGetConfigAttrib() returned error 0x" << std::hex << eglGetError();
		release_graphics();
		LOG(FATAL) << "Couldn't initialize graphics";
	}
	
	ANativeWindow_setBuffersGeometry(m_window, 0, 0, format);
	
	surface = eglCreateWindowSurface(display, config, m_window, 0);
	if (!surface)
	{
		LOG(ERR) << "eglCreateWindowSurface() returned error 0x" << std::hex << eglGetError();
		release_graphics();
		LOG(FATAL) << "Couldn't initialize graphics";
	}
	
	// Версия GL ES 2
	const EGLint attrib_list[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	context = eglCreateContext(display, config, 0, attrib_list);
	if (!context)
	{
		LOG(ERR) << "eglCreateContext() returned error 0x" << std::hex << eglGetError();
		release_graphics();
		LOG(FATAL) << "Couldn't initialize graphics";
	}
	
	if (!eglMakeCurrent(display, surface, surface, context))
	{
		LOG(ERR) << "eglMakeCurrent() returned error 0x" << std::hex << eglGetError();
		release_graphics();
		LOG(FATAL) << "Couldn't initialize graphics";
	}
	
	if (!eglQuerySurface(display, surface, EGL_WIDTH, &width) ||
		!eglQuerySurface(display, surface, EGL_HEIGHT, &height))
	{
		LOG(ERR) << "eglQuerySurface() returned error 0x" << std::hex << eglGetError();
		release_graphics();
		LOG(FATAL) << "Couldn't initialize graphics";
	}
	
	m_display = display;
	m_surface = surface;
	m_context = context;
	m_width = width;
	m_height = height;
}


void MainWindow::release_graphics()
{
	LOG(INFO) << "Destroying context";
	
	eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(m_display, m_context);
	eglDestroySurface(m_display, m_surface);
	eglTerminate(m_display);
	
	m_display = EGL_NO_DISPLAY;
	m_surface = EGL_NO_SURFACE;
	m_context = EGL_NO_CONTEXT;
	m_width = 0;
	m_height = 0;
}
