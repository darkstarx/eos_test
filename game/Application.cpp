#include <Application.hpp>
#include <graphics/Renderer.hpp>
#include "graphics/GObject.hpp"
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
	if (!renderer_alive()) return;
	graphics::GObjectSPtr root_obj = renderer().root_object();
	if (!root_obj) return;
	graphics::rotation_t rot = root_obj->rotation();
	rot.angle_y += 0.5f;
	root_obj->set_rotation(rot);
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
