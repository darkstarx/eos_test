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
	// ������������ ����� ����
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
	// ������� ����
	RECT window_rect;
	window_rect.top = window_rect.left = 0;
	window_rect.right = WINDOW_WIDTH;
	window_rect.bottom = WINDOW_HEIGHT;
	// ����������� ����� ����
	DWORD dwExStyle = 0;
	// ����� ����
	DWORD dwStyle = 0;
	// ����������� ����� (�������������/�������)
	if (false) {
		// ������������� �����
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = WINDOW_WIDTH;
		dmScreenSettings.dmPelsHeight = WINDOW_HEIGHT;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
		// �������� ������������� �����. ��� ������ ���� ����������.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			LOG(FATAL) << "Unable to switch to fullscreen mode";
		}
		// ����� ����
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		// � ������������� ������ �������� ������.
		ShowCursor(FALSE);
	} else {
		// ������� �����
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX;
	}
	// ������������� ������� � ����� ����
	AdjustWindowRectEx(&window_rect, dwStyle, FALSE, dwExStyle);
	// ������� ��������� ����
	m_window = CreateWindowEx(
		dwExStyle, m_class_name.c_str(), m_title.c_str(), dwStyle,
		0, 0, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top,
		NULL, NULL, m_hinst, NULL
	);
	if (!m_window)
		LOG(ERR) << "Cannot create the main window " << GetLastError();
	// ���������� ����
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
	// ������ �������� ������������ ���������
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags =
		PFD_DRAW_TO_WINDOW |	// �������� ����� � ����
		PFD_SUPPORT_OPENGL |	// ������ ������ ������������ OpenGL
		PFD_DOUBLEBUFFER;		// ����� ��������� ������� �����������
	pfd.iPixelType = PFD_TYPE_RGBA;		// ���������� RGBA ������
	pfd.cColorBits = 32;				// ������� ������ 32 ����
	// ����������� ��������
	HDC device_context = GetDC(m_window);
	if (!device_context)
		LOG(FATAL) << "Unable to create device context";
	// ������ ��������
	int pixel_format = ChoosePixelFormat(device_context, &pfd);
	// ����� �� ���������� ������ ��������, ��������� � pfd?
	if (!pixel_format)
		LOG(WARN) << "Unable to choose pixel format";
	// ������������� ������ ��������
	if (!SetPixelFormat(device_context, pixel_format, &pfd))
		LOG(FATAL) << "Unable to set choosen pixel format";
	// ������� GL-��������
	HGLRC glcontext = wglCreateContext(device_context);
	if (!glcontext)
		LOG(FATAL) << "Unable to create GL context";
	// ���������� GL-��������
	if (!wglMakeCurrent(device_context, glcontext))
		LOG(FATAL) << "Unable to activate GL context";

	// �������������� GLEW
	glewExperimental = GL_TRUE;
	GLenum status = glewInit();
	if (status != GLEW_OK)
		LOG(FATAL) << "GLEW Error: " << glewGetErrorString(status);

	// ��������� �������� � �������� GL-���������
	renderer().on_ctx_create();
	// �������� ��������� � ���, ��� ����������� ������������
	renderer().on_surface_changed(WINDOW_WIDTH, WINDOW_HEIGHT, true);
	
	const DWORD tick_interval = TICK_INTERVAL;
	DWORD next_tick_count = GetTickCount() + tick_interval;
	DWORD sleep_interval = tick_interval;

#ifndef NDEBUG
	int frames = 0;
	DWORD check_point = GetTickCount();
#endif
	// ��������� ����������� ����, ���� ���������� �� ����� �����������
	while (m_render_working)
	{
		// ��������� ������� ����������� �����
		renderer().tasks()->process_queue();
		// ��������� �����
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
	// ��������� �������� � ���������� GL-���������
	renderer().on_ctx_destroy();
	// ����������� ����� ���������������, ����������� GL-��������
	wglDeleteContext(glcontext);
	// ����������� ����������� ��������
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
	// ������� ����������
	app_create();
	// ������� ����������� ����
	init_graphics();
	// ��������� ����������
	app().start();
	
	const DWORD tick_interval = TICK_INTERVAL;
	DWORD next_tick_count = GetTickCount() + tick_interval;
	DWORD sleep_interval = tick_interval;

	MSG message;
	message.message = ~WM_QUIT;
	// ���� �������� ��������� �� �������, ���� �� ������ WM_QUIT
	while (message.message != WM_QUIT) {
		// ���� ����� ��������� ������� �������, ������� ��������� �� ���� ���
		const DWORD result = MsgWaitForMultipleObjectsEx(0, NULL, sleep_interval, QS_ALLEVENTS, 0);
		if (result != WAIT_TIMEOUT) {
			// ���� ������� �������� ��������� ���������� �� �� ��������
			while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
				// If a message was waiting in the message queue, process it
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
		// ���� ���� ������
		if (GetTickCount() >= next_tick_count) {
			app().tick();
			next_tick_count += tick_interval;
		}
		const DWORD current_tick_count = GetTickCount();
		// ����� �������� ��������� ������� ������� �� ������������ ��������� ������� ������� � ����
		sleep_interval = current_tick_count < next_tick_count ? next_tick_count - current_tick_count : 0;
		// ���� ����� ����� �������� ��������� �������� ����� ������
		if (sleep_interval > tick_interval) {
			sleep_interval = tick_interval;
			next_tick_count = GetTickCount() + tick_interval;
		}
	}

	// ���������� ���������������
	app().pause();
	app().stop();
	
	// ������������� ����������� ����� � ��������� ��������
	if (m_render_working) stop_rendering();
	
	// ��������� ����������
	app_destroy();
	
	// ���������������� �������
	deinit_graphics();
	
	// ��������� �������� ����� ��������
	return 0;
}
