#include <Application.hpp>
#include <utils/log.hpp>


Application* Application::m_instance = NULL;


void Application::create()
{
	ASSERT(!m_instance);
	m_instance = new Application();
	LOG(INFO) << "Application created";
}


void Application::destroy()
{
	ASSERT(m_instance);
	delete m_instance;
	m_instance = NULL;
	LOG(INFO) << "Application destroyed";
}


void Application::create_renderer()
{
	ASSERT(!m_renderer);
	m_renderer = new graphics::Renderer();
}


void Application::destroy_renderer()
{
	ASSERT(m_renderer);
	delete m_renderer;
	m_renderer = NULL;
}


void Application::tick()
{

}


void Application::start()
{

}


void Application::stop()
{

}


void Application::pause()
{

}


void Application::resume()
{

}


void Application::on_touch_action(touch_action_t touch_action, const graphics::position_t& position)
{
	
}


bool Application::on_key_down(keycode_t keycode)
{
	return false;
}


bool Application::on_key_up(keycode_t keycode)
{
	return false;
}


Application::Application()
: m_renderer(NULL)
{
// 	LOG(INFO) << "Application created";
}


Application::~Application()
{
// 	LOG(INFO) << "Application destroyed";
}
