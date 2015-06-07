#include <Application.hpp>
#include <graphics/Renderer.hpp>
#include <graphics/TextureManager.hpp>
#include <graphics/GContainer.hpp>
#include <graphics/GRectangle.hpp>
#include <graphics/GImage.hpp>
#include <graphics/GImages.hpp>
#include <resources/FileSystem.hpp>
#include <utils/log.hpp>
#include <utils/task_queue.hpp>
#include <memory/Cache.hpp>


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
	if (m_paused || m_stopped) return;
	m_task_queue->process_queue();
}


void Application::start()
{
	if (!m_stopped) return;
	m_stopped = false;
	// Ставим задачу на периодическую очистку кэша (раз в три секунды)
	m_task_cache_clean = m_task_queue->enqueue_repeatedly(std::bind(&Application::clean_cache, this), 3.0);
	
	DLOG(INFO) << "Application started";
}


void Application::stop()
{
	if (m_stopped) return;
	m_stopped = true;
	// Удаляем задачу на периодическую очистку кэша
	m_task_queue->unqueue(m_task_cache_clean);
	
	DLOG(INFO) << "Application stopped";
}


void Application::pause()
{
	if (m_paused) return;
	m_paused = true;
	DLOG(INFO) << "Application paused";
}


void Application::resume()
{
	if (!m_paused) return;
	m_paused = false;
	DLOG(INFO) << "Application resumed";
}


void Application::on_graphics_created()
{
	DLOG(INFO) << "Application receives graphics creation";
	if (m_scene) return;
	
	m_scene.reset(new graphics::GContainer());
	renderer().set_graphics(m_scene);
	m_rect.reset(new graphics::GRectangle(graphics::rectangle_t(350.0f, 350.0f, 120.0f, 120.0f)));
	m_rect->set_transform_point(graphics::position_t(60.0f, 60.0f));
	m_rect->set_color(graphics::color_t(0.3f, 1.0f, 1.0f, 0.3f));
	
	m_img.reset(new graphics::GImage(graphics::rectangle_t(0, 0, 1196, 720)));
	graphics::TextureSPtr tex = texmgr().get_texture_from_asset("test.png");
	m_img->set_image(tex);
	
	m_imgs.reset(new graphics::GImages(graphics::position_t(140.0f, 125.0f)));
	m_imgs->set_image(tex);
	m_imgs->add_region(graphics::region_t(graphics::rectangle_t(0, 0, 100, 100), graphics::rectangle_t(200, 250, 300, 300), graphics::rectangle_t(0, 0, 0, 0)));
	m_imgs->add_region(graphics::region_t(graphics::rectangle_t(150, 150, 130, 100), graphics::rectangle_t(150, 70, 260, 200), graphics::rectangle_t(0, 0, 0, 0)));
	m_imgs->set_transform_point(graphics::position_t(140.0f, 125.0f));
	
	m_scene->add_object(m_img);
	m_scene->add_object(m_imgs);
	m_scene->add_object(m_rect);
	
	m_task = renderer().tasks()->enqueue_repeatedly(std::bind(&Application::rotate_rect, this), 1.0 / 60.0);
}


void Application::on_graphics_destroyed()
{
	DLOG(INFO) << "Application receives graphics destruction";
}


void Application::on_touch_action(touch_action_t touch_action, const graphics::position_t& position)
{
	DLOG(INFO) << "Application receives touch action";
}


bool Application::on_key_down(keycode_t keycode)
{
	DLOG(INFO) << "Application receivs key_down action";
	return false;
}


bool Application::on_key_up(keycode_t keycode)
{
	DLOG(INFO) << "Application receivs key_up action";
	return false;
}


void Application::rotate_rect()
{
	if (!m_rect) return;
	graphics::rotation_t rot = m_rect->rotation();
	rot.angle_z += 0.8f;
	if (rot.angle_z > 360.0f) rot.angle_z = 0.0f;
	m_rect->set_rotation(rot);
	m_imgs->set_rotation(graphics::rotation_t(0.0f, 0.0f, -rot.angle_z));
}


Application::Application()
: m_task_queue(new utils::task_queue)
, m_paused(true)
, m_stopped(true)
{
	// Приложение создано
	DLOG(INFO) << "Application created";
	// Создаем кэш
	memory::Cache::create();
	// Создаем экземпляр доступа к файловой системе
	FileSystem::create();
	// Создаем текстурный менеджер
	graphics::TextureManager::create();
}


Application::~Application()
{
	// Отменяем задачи
	renderer().tasks()->unqueue(m_task);
	// Разрушаем сцену
	m_scene.reset();
	m_img.reset();
	m_imgs.reset();
	m_rect.reset();
	// Прекращаем отрисовку сцены
	renderer().reset_graphics();
	// Полностью очищаем кэш
	cache().force_clean();
	// Разрушаем текстурный менеджер
	graphics::TextureManager::destroy();
	// Разрушаем экземпляр доступа к файловой системе
	FileSystem::destroy();
	// Разрушаем кэш
	memory::Cache::destroy();
	// Приложение разрушено
	DLOG(INFO) << "Application destroyed";
}


void Application::clean_cache()
{
	cache().clean();
}
