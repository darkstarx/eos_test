#include <Application.hpp>
#include <graphics/Renderer.hpp>
#include <graphics/GContainer.hpp>
#include <graphics/GRectangle.hpp>
#include <resources/FileSystem.hpp>
#include <utils/log.hpp>


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
	if (!m_rect) return;
	graphics::rotation_t rot = m_rect->rotation();
	rot.angle_z += 0.8f;
	m_rect->set_rotation(rot);
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


void Application::on_renderer_created()
{
	LOG(INFO) << "Application got signal renderer created";
	graphics::GContainerSPtr scene(new graphics::GContainer());
	renderer().set_graphics(scene);
	m_rect.reset(new graphics::GRectangle(graphics::rectangle_t(350.0f, 350.0f, 80.0f, 80.0f)));
	m_rect->set_transform_point(graphics::position_t(40.0f, 40.0f));
	m_rect->set_visible(true);
	m_rect->set_color(graphics::color_t(1.0f, 1.0f, 0.5f, 1.0f));
	scene->add_object(m_rect);
}


void Application::on_renderer_destroyed()
{
	m_rect.reset();
	LOG(INFO) << "Application got signal renderer destroyed";
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
