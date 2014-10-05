#include "Renderer.h"
#include "game/Application.h"
#include "game/ApplicationGUI.h"
#include "game/graphics/gl.hpp"
#include "game/utils/log.h"
#include <chrono>


Renderer::Renderer(HDC hDC, const dimension_t &size)
: m_hDC(hDC)
, m_size(size)
, m_working(false)
, m_thread()
{
}


Renderer::~Renderer()
{
	stop();
}


void Renderer::start()
{
	if (m_working) return;
	m_working = true;
	m_thread.reset(new std::thread(std::bind(&Renderer::work, this)));
}


void Renderer::stop()
{
	if (!m_working) return;
	m_working = false;
	m_thread->join();
	m_thread.reset();
}


void Renderer::work()
{
	// ������� render context (RC)
	HGLRC hRC = wglCreateContext(m_hDC);
	bool is_ok = wglMakeCurrent(m_hDC, hRC);
	LOG(INFO) << "�������� ��������� OpenGL " << (is_ok ? "ok" : "err");

	// �������������� �������
	glewInit();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, m_size.w, m_size.h, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// �������������� ����������
	Application::instance().initialize();
	Application::instance().m_GUI->MapViewer->m_center.x = Application::instance().m_GUI->MapViewer->m_player->m_cell->x;
	Application::instance().m_GUI->MapViewer->m_center.y = Application::instance().m_GUI->MapViewer->m_player->m_cell->y;
	// ��������� ������ ����������
	Application::instance().start();

	// ��� �������� fps ���������� ������������ fps = 60
	std::chrono::time_point<std::chrono::system_clock> start, end;
	const std::chrono::milliseconds normal_duration(1000/60);
	// ��������� ����������� ����, ���� renderer �� ����� ����������
	while (m_working)
	{
		start = std::chrono::system_clock::now();
		Application::instance().m_GUI->MapViewer->update();
		Application::instance().render();
		SwapBuffers(m_hDC);
		end = std::chrono::system_clock::now();

		// ���� �� ��������� ���� ������ normal_duration, ������� �������� �� ������ ����
		if (start + normal_duration < end) continue;
		std::chrono::microseconds duration(std::chrono::duration_cast<std::chrono::microseconds>(end - start));
		std::this_thread::sleep_for(normal_duration - duration);
	}
	// renderer ����������, ������������� ������ ����������
	Application::instance().stop();

	// ��������� ����������� �������� OpenGL
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
}