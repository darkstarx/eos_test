#include <Application.hpp>
#include <graphics/Renderer.hpp>
#include <graphics/GContainer.hpp>
#include <graphics/GBox.hpp>
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
	if (!m_box) return;
	graphics::rotation_t rot = m_box->rotation();
	graphics::box_t box = m_box->box();
	rot.angle_y += 0.8f;
	m_box->set_rotation(rot);
	box.y += 0.0005f;
	m_box->set_box(box);
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
	m_box.reset(new graphics::GBox(graphics::box_t(-0.2f, -0.5f, 1.0f, 0.4f, 0.4f, 0.4f)));
	m_box->set_transform_point(graphics::position_t(0.2f, 0.2f, 0.2f));
	m_box->set_visible(true);
	m_box->set_color(graphics::color_t(1.0f, 1.0f, 0.5f, 1.0f));
	scene->add_object(m_box);
}


void Application::on_renderer_destroyed()
{
	m_box.reset();
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
