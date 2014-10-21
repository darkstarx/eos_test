#include <graphics/GObject.hpp>
#include <graphics/Renderer.hpp>
#include <graphics/ShaderProgram.hpp>
#include <graphics/MatrixStack.hpp>


namespace graphics
{
	
	GObject::GObject()
	: m_vertices_count(0)
	, m_indices_count(0)
	, m_visible(true)
	, m_vertices_valid(false)
	, m_indices_valid(false)
	{
	}
	
	
	GObject::~GObject()
	{
	}
	
	
	void GObject::set_visible(bool value)
	{
		if (m_visible == value) return;
		utils::scoped_lock guard(m_lock);
		m_visible = value;
	}
	
	
	void GObject::set_color(const color_t& value)
	{
		if (m_color == value) return;
		utils::scoped_lock guard(m_lock);
		m_color = value;
	}
	
	
	void GObject::set_transform_point(const position_t& value)
	{
		if (m_transform_point == value) return;
		utils::scoped_lock guard(m_lock);
		m_transform_point = value;
	}
	
	
	void GObject::set_rotation(const rotation_t& value)
	{
		if (m_rotation == value) return;
		utils::scoped_lock guard(m_lock);
		m_rotation = value;
	}
	
	
	void GObject::set_scale(const scale_t& value)
	{
		if (m_scale == value) return;
		utils::scoped_lock guard(m_lock);
		m_scale = value;
	}
	
	
	void GObject::draw()
	{
		utils::scoped_lock guard(m_lock);
		// Невидимые объекты не отображаются
		if (!m_visible) return;
		// Обновляем графические данные, если они не актуальны
		if (!m_vertices_valid) update_vertices();
		if (!m_indices_valid) update_indices();
		// Запоминаем текущую матрицу, она будет трансформироваться
		matrix().push();
		// 
		before_draw();
		// Определяем шейдерную программу
		set_program();
		// Рисуем объект
		do_draw();
		// 
		after_draw();
		// Восстанавливаем матрицу, удаляя все текущие трансформации
		matrix().pop();
		// Отключаем буферы от шейдерной программы
		if (m_program)
			m_program->reset_attributes();
	}
	
	
	void GObject::update_vertices()
	{
		m_vertices_valid = true;
	}
	
	
	void GObject::update_indices()
	{
		m_indices_valid = true;
	}
	
	
	void GObject::before_draw()
	{
	}
	
	
	void GObject::set_program()
	{
		// Если шейдерная программа не определена, используем простую программу
		if (!m_program)
			m_program = renderer().get_program(sp_simple);
		m_program->use();
		m_program->set_attribute_position(m_vertices.get());
		m_program->set_uniform_mvpmatrix(matrix().get());
		m_program->set_uniform_color(m_color);
// 		m_program->set_uniform_texture(0);
	}
	
	
	void GObject::after_draw()
	{
	}
	
}