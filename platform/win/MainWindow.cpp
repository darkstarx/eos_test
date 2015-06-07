#include <MainWindow.hpp>
#include <Application.hpp>
#include <graphics/Renderer.hpp>
#include <utils/log.hpp>
#include <utils/task_queue.hpp>


const DWORD MainWindow::TICK_INTERVAL = 1000 / 60;

HINSTANCE MainWindow::m_hinst = 0;
std::string MainWindow::m_title = "Explorers of Saarum";
std::string MainWindow::m_class_name = "EoSMainWindow";
HWND MainWindow::m_window = 0;
std::atomic<bool> MainWindow::m_render_working;
std::shared_ptr<std::thread> MainWindow::m_render_thread;

void MainWindow::init_graphics()
{
	// Регистрируем класс окна
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hinst;
	wcex.hIcon = NULL;
	wcex.hIconSm = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = 0;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = m_class_name.c_str();
	wcex.lpfnWndProc = MainWindow::event_handler;

	if (!RegisterClassEx(&wcex)) {
		LOG(FATAL) << "Unable to register class of main window";
	}
	// Размеры окна
	RECT window_rect;
	window_rect.top = window_rect.left = 0;
	window_rect.right = WINDOW_WIDTH;
	window_rect.bottom = WINDOW_HEIGHT;
	// Расширенный стиль окна
	DWORD dwExStyle = 0;
	// Стиль окна
	DWORD dwStyle = 0;
	// Графический режим (полноэкранный/оконный)
	if (false) {
		// Полноэкранный режим
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = WINDOW_WIDTH;
		dmScreenSettings.dmPelsHeight = WINDOW_HEIGHT;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
		// Включаем полноэкранный режим. При ошибке срем исключение.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			LOG(FATAL) << "Unable to switch to fullscreen mode";
		}
		// Стиль окна
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		// В полноэкранном режиме скрываем курсор.
		ShowCursor(FALSE);
	} else {
		// Оконный режим
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX;
	}
	// Устанавливаем размеры и стиль окна
	AdjustWindowRectEx(&window_rect, dwStyle, FALSE, dwExStyle);
	// Создаем экземпляр окна
	m_window = CreateWindowEx(
		dwExStyle, m_class_name.c_str(), m_title.c_str(), dwStyle,
		0, 0, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top,
		NULL, NULL, m_hinst, NULL
	);
	if (!m_window)
		LOG(ERR) << "Cannot create the main window " << GetLastError();
	// Показываем окно
	ShowWindow(m_window, SW_SHOW);
}


void MainWindow::deinit_graphics()
{
	DestroyWindow(m_window);
	m_window = 0;
	UnregisterClass(m_class_name.c_str(), m_hinst);
}


LRESULT CALLBACK MainWindow::event_handler(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message) {
		case WM_SIZE:
			if (wparam == SIZE_MINIMIZED) {
				app().pause();
				stop_rendering();
			}
			else if (wparam == SIZE_RESTORED) {
				start_rendering();
				app().resume();
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc(handle, message, wparam, lparam);
}


void MainWindow::graphics_worker()
{
	// Формат пикселов графического контекста
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags =
		PFD_DRAW_TO_WINDOW |	// Рисовать будем в окно
		PFD_SUPPORT_OPENGL |	// Формат должен поддерживать OpenGL
		PFD_DOUBLEBUFFER;		// Нужна поддержка двойной буферизации
	pfd.iPixelType = PFD_TYPE_RGBA;		// Используем RGBA формат
	pfd.cColorBits = 32;				// Глубина цваета 32 бита
	// Графический контекст
	HDC device_context = GetDC(m_window);
	if (!device_context)
		LOG(FATAL) << "Unable to create device context";
	// Формат пикселов
	int pixel_format = ChoosePixelFormat(device_context, &pfd);
	// Можно ли установить формат пикселов, указанный в pfd?
	if (!pixel_format)
		LOG(WARN) << "Unable to choose pixel format";
	// Устанавливаем формат пикселов
	if (!SetPixelFormat(device_context, pixel_format, &pfd))
		LOG(FATAL) << "Unable to set choosen pixel format";
	// Создаем GL-контекст
	HGLRC glcontext = wglCreateContext(device_context);
	if (!glcontext)
		LOG(FATAL) << "Unable to create GL context";
	// Активируем GL-контекст
	if (!wglMakeCurrent(device_context, glcontext))
		LOG(FATAL) << "Unable to activate GL context";

	// Инициализируем GLEW
	glewExperimental = GL_TRUE;
	GLenum status = glewInit();
	if (status != GLEW_OK)
		LOG(FATAL) << "GLEW Error: " << glewGetErrorString(status);

	// Оповещаем рендерер о создании GL-контекста
	renderer().on_ctx_create();
	// Сообщаем рендереру о том, что поверхность сформирована
	renderer().on_surface_changed(WINDOW_WIDTH, WINDOW_HEIGHT, true);
	
	const DWORD tick_interval = TICK_INTERVAL;
	DWORD next_tick_count = GetTickCount() + tick_interval;
	DWORD sleep_interval = tick_interval;

#ifndef NDEBUG
	int frames = 0;
	DWORD check_point = GetTickCount();
#endif
	// Выполняем графический цикл, пока приложение не будет остановлено
	while (m_render_working)
	{
		// Обработка очереди графических задач
		renderer().tasks()->process_queue();
		// Отрисовка сцены
		if (!renderer().is_valid())
		{
			if (renderer().on_draw_frame()) {
				SwapBuffers(device_context);
#ifndef NDEBUG
				++frames;
#endif
			}
		}
		next_tick_count += tick_interval;
		const DWORD current_tick_count = GetTickCount();
#ifndef NDEBUG
		if (current_tick_count - check_point >= 1000) {
			DLOG(INFO) << "fps: " << frames;
			frames = 0;
			check_point = current_tick_count;
		}
#endif
		sleep_interval = current_tick_count < next_tick_count ? next_tick_count - current_tick_count : 0;
		if (sleep_interval > tick_interval) {
			sleep_interval = tick_interval;
			next_tick_count = GetTickCount() + tick_interval;
		}
		if (sleep_interval) {
			Sleep(sleep_interval);
		}
	}
	// Оповещаем рендерер о разрушении GL-контекста
	renderer().on_ctx_destroy();
	// Графический поток останавливается, освобождаем GL-контекст
	wglDeleteContext(glcontext);
	// Освобождаем графический контекст
	ReleaseDC(m_window, device_context);
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


int MainWindow::create_window(HINSTANCE hinst)
{
	m_hinst = hinst;
	// Создаем приложение
	app_create();
	// Создаем графическое окно
	init_graphics();
	// Запускаем приложение
	app().start();
	
	const DWORD tick_interval = TICK_INTERVAL;
	DWORD next_tick_count = GetTickCount() + tick_interval;
	DWORD sleep_interval = tick_interval;

	MSG message;
	message.message = ~WM_QUIT;
	// Цикл ожидания сообщений от системы, пока не придет WM_QUIT
	while (message.message != WM_QUIT) {
		// Ждем новых сообщений столько времени, сколько требуется на один тик
		const DWORD result = MsgWaitForMultipleObjectsEx(0, NULL, sleep_interval, QS_ALLEVENTS, 0);
		if (result != WAIT_TIMEOUT) {
			// Если функция ожидания сообщений прервалась не по таймауту
			while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
				// If a message was waiting in the message queue, process it
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
		// Если пора тикать
		if (GetTickCount() >= next_tick_count) {
			app().tick();
			next_tick_count += tick_interval;
		}
		const DWORD current_tick_count = GetTickCount();
		// Время ожидания следующих событий зависит от длительности обработки текущих событий и тика
		sleep_interval = current_tick_count < next_tick_count ? next_tick_count - current_tick_count : 0;
		// Если вдруг время ожидания превышает интервал между тиками
		if (sleep_interval > tick_interval) {
			sleep_interval = tick_interval;
			next_tick_count = GetTickCount() + tick_interval;
		}
	}

	// Приложение останавливается
	app().pause();
	app().stop();
	
	// Останавливаем графический поток и разрушаем рендерер
	if (m_render_working) stop_rendering();
	
	// Разрушаем приложение
	app_destroy();
	
	// Деинициализируем графику
	deinit_graphics();
	
	// Завершаем основной поток процесса
	return 0;
}
