#include <graphics/Renderer.hpp>
#include <graphics/gl.hpp>
#include <graphics/Context.hpp>
#include <graphics/ShaderProgram.hpp>
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
		// Загружаем шейдеры
		load_shaders();
		
		// Настраиваем отображение сцены
		glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
// 		glClearDepth(1.0);
// 		glDepthFunc(GL_LESS);
// 		glEnable(GL_DEPTH_TEST);
// 		glShadeModel(GL_SMOOTH);
// 		glMatrixMode(GL_PROJECTION);
// 		glLoadIdentity();
// 		gluPerspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
// 		glMatrixMode(GL_MODELVIEW);
	}
	
	
	void Renderer::on_draw_frame()
	{
		// Если сцена не изменилась, не нужно перерисовывать кадр
		if (m_valid) return;
		
		// Перерисовка кадра
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_xr -= 0.5;
		m_yr -= 0.5;
		m_zr -= 0.5;
		draw_cube();
		glFlush();
		
		// Сцена отрисована
// 		m_valid = true;
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
		
// 		glLoadIdentity();
// 		glTranslatef(0.0f, 0.0f, -7.0f);    // Сдвинуть вглубь экрана
// 		
// 		glRotatef(m_xr, 1.0f, 0.0f, 0.0f);   // Вращение куба по X, Y, Z
// 		glRotatef(m_yr, 0.0f, 1.0f, 0.0f);   // Вращение куба по X, Y, Z
// 		glRotatef(m_zr, 0.0f, 0.0f, 1.0f);   // Вращение куба по X, Y, Z
// 		
// 		glBegin(GL_QUADS);                  // Рисуем куб
// 		
// 		glColor3f(0.0f, 1.0f, 0.0f);        // Синяя сторона (Верхняя)
// 		glVertex3f( 1.0f, 1.0f, -1.0f);     // Верхний правый угол квадрата
// 		glVertex3f(-1.0f, 1.0f, -1.0f);     // Верхний левый
// 		glVertex3f(-1.0f, 1.0f,  1.0f);     // Нижний левый
// 		glVertex3f( 1.0f, 1.0f,  1.0f);     // Нижний правый
// 		
// 		glColor3f(1.0f, 0.5f, 0.0f);        // Оранжевая сторона (Нижняя)
// 		glVertex3f( 1.0f, -1.0f,  1.0f);    // Верхний правый угол квадрата
// 		glVertex3f(-1.0f, -1.0f,  1.0f);    // Верхний левый
// 		glVertex3f(-1.0f, -1.0f, -1.0f);    // Нижний левый
// 		glVertex3f( 1.0f, -1.0f, -1.0f);    // Нижний правый
// 		
// 		glColor3f(1.0f, 0.0f, 0.0f);        // Красная сторона (Передняя)
// 		glVertex3f( 1.0f,  1.0f, 1.0f);     // Верхний правый угол квадрата
// 		glVertex3f(-1.0f,  1.0f, 1.0f);     // Верхний левый
// 		glVertex3f(-1.0f, -1.0f, 1.0f);     // Нижний левый
// 		glVertex3f( 1.0f, -1.0f, 1.0f);     // Нижний правый
// 		
// 		glColor3f(1.0f,1.0f,0.0f);          // Желтая сторона (Задняя)
// 		glVertex3f( 1.0f, -1.0f, -1.0f);    // Верхний правый угол квадрата
// 		glVertex3f(-1.0f, -1.0f, -1.0f);    // Верхний левый
// 		glVertex3f(-1.0f,  1.0f, -1.0f);    // Нижний левый
// 		glVertex3f( 1.0f,  1.0f, -1.0f);    // Нижний правый
// 		
// 		glColor3f(0.0f,0.0f,1.0f);          // Синяя сторона (Левая)
// 		glVertex3f(-1.0f,  1.0f,  1.0f);    // Верхний правый угол квадрата
// 		glVertex3f(-1.0f,  1.0f, -1.0f);    // Верхний левый
// 		glVertex3f(-1.0f, -1.0f, -1.0f);    // Нижний левый
// 		glVertex3f(-1.0f, -1.0f,  1.0f);    // Нижний правый
// 		
// 		glColor3f(1.0f,0.0f,1.0f);          // Фиолетовая сторона (Правая)
// 		glVertex3f( 1.0f,  1.0f, -1.0f);    // Верхний правый угол квадрата
// 		glVertex3f( 1.0f,  1.0f,  1.0f);    // Верхний левый
// 		glVertex3f( 1.0f, -1.0f,  1.0f);    // Нижний левый
// 		glVertex3f( 1.0f, -1.0f, -1.0f);    // Нижний правый
// 		
// 		glEnd();                            // Закончили квадраты   
	}
	
	
	Renderer::Renderer()
	: m_valid(false)
	, m_xr(0)
	, m_yr(0)
	, m_zr(0)
	{
		ctx_create();
		LOG(INFO) << "Renderer created";
	}
	
	
	Renderer::~Renderer()
	{
		ctx_destroy();
		LOG(INFO) << "Renderer destroyed";
	}
	
}