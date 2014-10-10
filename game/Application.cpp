#include <Application.hpp>
#include <utils/log.hpp>
#include <resources/FileSystem.hpp>


Application* Application::m_instance = NULL;


void Application::create()
{
	ASSERT(!m_instance);
	m_instance = new Application();
}


void Application::destroy()
{
	ASSERT(m_instance);
	delete m_instance;
	m_instance = NULL;
}


void Application::tick()
{
}


void Application::start()
{
	if (!m_stopped) return;
	m_stopped = false;
	LOG(INFO) << "Application started";
}


void Application::stop()
{
	if (m_stopped) return;
	m_stopped = true;
	LOG(INFO) << "Application stopped";
}


void Application::pause()
{
	if (m_paused) return;
	m_paused = true;
	LOG(INFO) << "Application paused";
}


void Application::resume()
{
	if (!m_paused) return;
	m_paused = false;
	LOG(INFO) << "Application resumed";
}


void Application::on_touch_action(touch_action_t touch_action, const graphics::position_t& position)
{
	LOG(INFO) << "Application receivs touch action";
}


bool Application::on_key_down(keycode_t keycode)
{
	LOG(INFO) << "Application receivs key_down action";
	return false;
}


bool Application::on_key_up(keycode_t keycode)
{
	LOG(INFO) << "Application receivs key_up action";
	return false;
}


Application::Application()
: m_paused(true)
, m_stopped(true)
{
	LOG(INFO) << "Application created";
}


Application::~Application()
{
	LOG(INFO) << "Application destroyed";
}
