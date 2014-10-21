#include <graphics/Renderer.hpp>
#include <graphics/gl.hpp>
#include <graphics/Context.hpp>
#include <graphics/ShaderProgram.hpp>
#include <graphics/GBox.hpp>
#include "MatrixStack.hpp"
#include <resources/FileSystem.hpp>
#include <utils/log.hpp>


namespace graphics
{
	
	Renderer *Renderer::m_instance = NULL;
	
	
	void Renderer::create()
	{
		ASSERT(!m_instance);
		m_instance = new Renderer();
	}
	
	
	void Renderer::destroy()
	{
		ASSERT(m_instance);
		delete m_instance;
		m_instance = NULL;
	}
	
	
	void Renderer::on_surface_changed(int width, int height, bool is_tablet)
	{
		DLOG(INFO) << "Surface changed: " << width << "x" << height << ", tablet: " << std::boolalpha << is_tablet;
		// Загружаем шейдеры
		load_shaders();
		
		// Настраиваем отображение сцены
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glViewport(0, 0, width, height);
		
		// Enable depth buffer
// 		glEnable(GL_DEPTH_TEST);
// 		glClearDepth(1.0);
// 		glDepthFunc(GL_LESS);
		// Enable back face culling
// 		glEnable(GL_CULL_FACE);
		
		// Матрица проекции
		matrix().load_identity();
		float aspect = (float)width / (float)height;
// 		matrix().ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
// 		matrix().frustum(0.0f, width, 0.0f, height, -100.0f, 100.0f);
// 		matrix().frustum(-aspect, aspect, -1.0f, 1.0f, 1.0f, 100.0f);
		matrix().perspective(45.0f, aspect, 0.0001f, 100.0f); // если zNear=0 то ничего не рисует, пока не выяснено почему
	}
	
	
	void Renderer::on_draw_frame()
	{
		// Если сцена не изменилась, не нужно перерисовывать кадр
		if (m_valid) return;
		
		// Перерисовка кадра
		glClear(GL_COLOR_BUFFER_BIT/* | GL_DEPTH_BUFFER_BIT*/);
		draw_cube();
		glFlush();
		
		// Сцена отрисована
// 		m_valid = true;
	}
	
	
	ShaderProgramSPtr Renderer::get_program(shader_program_t program_type)
	{
		return m_shader_programs[program_type];
	}
	
	
	void Renderer::load_shaders()
	{
		// Простая шейдерная программа
		{
			utils::bytearray src_vert;
			utils::bytearray src_frag;
			const std::string src_vert_path("simple.vsh");
			const std::string src_frag_path("simple.fsh");
			if (!resources().load_asset(src_vert_path, src_vert))
				LOG(FATAL) << "Ошибка загрузки шейдера " << src_vert_path;
			if (!resources().load_asset(src_frag_path, src_frag))
				LOG(FATAL) << "Ошибка загрузки шейдера " << src_frag_path;
			
			ShaderProgramSPtr simple_program(new ShaderProgram(src_vert, src_frag));
			m_shader_programs[sp_simple] = simple_program;
		}
		
	}
	
	
	void Renderer::release_shaders()
	{
		m_shader_programs.clear();
	}
	
	
	void Renderer::draw_cube()
	{
		m_root_object->draw();
	}
	
	
	Renderer::Renderer()
	: m_valid(false)
	{
		ctx_create();
		LOG(INFO) << "Renderer created";
		
		m_root_object.reset(new GBox(box_t(-0.2f, -0.2f, -1.0f, 0.4f, 0.4f, 0.4f)));
		m_root_object->set_transform_point(position_t(0.2f, 0.2f, 0.2f));
		m_root_object->set_visible(true);
		m_root_object->set_color(color_t(1.0f, 1.0f, 0.5f, 1.0f));
	}
	
	
	Renderer::~Renderer()
	{
		ctx_destroy();
		LOG(INFO) << "Renderer destroyed";
	}
	
}