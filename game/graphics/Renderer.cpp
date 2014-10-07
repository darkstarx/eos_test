#include <graphics/Renderer.hpp>
#include <graphics/gl.hpp>
#include <utils/log.hpp>


namespace graphics {
	
	Renderer::Renderer()
	: m_valid(false)
	, m_xr(0)
	, m_yr(0)
	, m_zr(0)
	{
		
	}
	
	
	Renderer::~Renderer()
	{
		
	}
	
	
	void Renderer::on_surface_changed(int width, int height, bool is_tablet)
	{
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
	
}