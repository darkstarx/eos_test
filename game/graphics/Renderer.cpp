#include <graphics/Renderer.hpp>
#include <graphics/gl.hpp>
#include <graphics/Context.hpp>
#include <graphics/ShaderProgram.hpp>
#include <graphics/GContainer.hpp>
#include "MatrixStack.hpp"
#include <resources/FileSystem.hpp>


namespace graphics
{
	
	Renderer* Renderer::m_instance = NULL;
	RendererDestroyer Renderer::m_destroyer;
	
	/** @class RendererDestroyer */
	RendererDestroyer::~RendererDestroyer()
	{
		ASSERT(m_instance);
		m_instance->m_instance = NULL;
		delete m_instance;
	}
	
	
	
	/** \class Renderer */
	
	Renderer& Renderer::instance()
	{
		if (!m_instance)
		{
			m_instance = new Renderer();
			m_destroyer.init(m_instance);
		}
		return *m_instance;
	}
	
	
	void Renderer::on_ctx_create()
	{
		ctx_create();
		if (app_alive()) app().on_graphics_created();
	}
	
	
	void Renderer::on_ctx_destroy()
	{
		if (app_alive()) app().on_graphics_destroyed();
		ctx_destroy();
	}
	
	
	void Renderer::set_graphics(const GContainerSPtr& container)
	{
		if (m_graphics == container) return;
		// Переопределение первичного графического контейнера не является атомарным, защищаем смену
		// контейнера при его переопределении
		utils::scoped_lock guard(m_graphics_lock);
		if (m_graphics) m_graphics->set_prime(false);
		m_graphics = container;
		if (m_graphics) m_graphics->set_prime(true);
		// После смены графики экран следует обновить
		m_valid = false;
	}
	
	
	void Renderer::invalidate()
	{
		utils::scoped_lock guard(m_graphics_lock);
		m_valid = false;
	}
	
	
	void Renderer::on_surface_changed(int width, int height, bool is_tablet)
	{
		DLOG(INFO) << "Surface changed: " << width << "x" << height << ", tablet: " << std::boolalpha << is_tablet;
		invalidate();
		
		// Устанавливаем вьюпорт принудительно, так как gl-контекст между сменами поверхности мог быть разрушен
		gl::set_viewport(rectangle_t(0, 0, width, height), true);
		
		// Запоминаем размеры экрана. Если размеры поверхности не изменились, матрицы не изменяем
		if (!gl::set_screen_size(dimension_t(width, height))) return;
		
		// Строим исходную матрицу проекции, от которой будут отталкиваться координаты объектов
		matrix().reset();
/*
		// Аспект - отношение ширины вьюпорта к его высоте
		float aspect = (float)width / (float)height;
		// Перспектива: ближняя и дальняя плоскости отсечения всегда положительные, ось Z направлена из глубины в
		// сторону экрана, позиция камеры в нулевой точке мировых координат, смотрит в глубину экрана, расположена
		// в горизонтальной плоскости осей X и Z.
		matrix().perspective(45.0f, aspect, 0.0001f, 100.0f);
		// Камеру разворачиваем вокруг оси Z в обратную сторону, чтобы смотрела по направлению оси Z. Таким образом
		// добиваемся того, что ось Z направлена от экрана в глубину, а не наоборот.
		matrix().look_at(position_t(0.0f, 0.0f, 0.0f), position_t(0.0f, 0.0f, 1.0f), position_t(0.0f, 1.0f, 0.0f));
*/
		matrix().ortho(0.0f, static_cast<GLfloat>(width), 0.0f, static_cast<GLfloat>(height), -100.0f, 100.0f);
	}
	
	
	void Renderer::on_draw_frame()
	{
		// Проверка на необходимость обновления экрана
		{
			utils::scoped_lock guard(m_graphics_lock);
			if (m_valid) return;
			m_valid = true;
		}
		// Очистить экран следует даже если графика не определена
		ctx().clear_buffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Если графика не определена, то рисовать нечего
		if (!m_graphics) return;
		// Перерисовка кадра
		m_graphics->draw();
	}
	
	
	ShaderProgramSPtr Renderer::get_program(shader_program_t program_type)
	{
		return m_shader_programs[program_type];
	}
	
	
	void Renderer::load_shaders()
	{
		// Простая шейдерная программа для вершин из двух элементов
		{
			utils::bytearray src_vert;
			utils::bytearray src_frag;
			const std::string src_vert_path("simple.vsh");
			const std::string src_frag_path("simple.fsh");
			if (!resources().load_asset(src_vert_path, src_vert))
				LOG(FATAL) << "Ошибка загрузки шейдера " << src_vert_path;
			if (!resources().load_asset(src_frag_path, src_frag))
				LOG(FATAL) << "Ошибка загрузки шейдера " << src_frag_path;
			
			ShaderProgramSPtr &program = m_shader_programs[sp_simple];
			if (program) {
				program->initialize(src_vert, src_frag);
			} else {
				program.reset(new ShaderProgram(src_vert, src_frag));
			}
		}
		
		// Простая шейдерная программа для вершин из трех элементов
		{
			utils::bytearray src_vert;
			utils::bytearray src_frag;
			const std::string src_vert_path("simple3d.vsh");
			const std::string src_frag_path("simple3d.fsh");
			if (!resources().load_asset(src_vert_path, src_vert))
				LOG(FATAL) << "Ошибка загрузки шейдера " << src_vert_path;
			if (!resources().load_asset(src_frag_path, src_frag))
				LOG(FATAL) << "Ошибка загрузки шейдера " << src_frag_path;
			
			ShaderProgramSPtr &program = m_shader_programs[sp_simple3d];
			if (program) {
				program->initialize(src_vert, src_frag);
			} else {
				program.reset(new ShaderProgram(src_vert, src_frag));
			}
		}
		
	}
	
	
	void Renderer::release_shaders()
	{
		for (shader_programs_t::iterator it = m_shader_programs.begin(), end = m_shader_programs.end(); it != end; ++it) {
			it->second->deinitialize();
		}
	}
	
	
	Renderer::Renderer()
	: m_valid(false)
	{
		LOG(INFO) << "Renderer created";
	}
	
	
	Renderer::~Renderer()
	{
		LOG(INFO) << "Renderer destroyed";
	}
	
}