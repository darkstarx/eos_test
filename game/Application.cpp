#include "Application.hpp"
#include <assert.h>


Application* Application::m_instance = NULL;

void Application::create()
{
	assert(!m_instance);
	m_instance = new Application();
}

void Application::destroy()
{
	assert(m_instance);
	delete m_instance;
}

Application::Application()
{

}

Application::~Application()
{

}
