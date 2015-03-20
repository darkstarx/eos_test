#include <graphics/GObject.hpp>
#include <graphics/Renderer.hpp>
#include <graphics/ShaderProgram.hpp>
#include <graphics/ObjectsContainer.hpp>
#include <graphics/MatrixStack.hpp>


namespace graphics
{
	
	GObject::GObject()
	: m_owner(0)
	, m_color(graphics::color_t(1.0f, 1.0f, 1.0f, 1.0f))
	, m_visible(true)
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
		invalidate();
	}
	
	
	void GObject::set_color(const color_t& value)
	{
		if (m_color == value) return;
		utils::scoped_lock guard(m_lock);
		m_color = value;
		invalidate();
	}
	
	
	void GObject::set_opacity(opacity_t value)
	{
		if (m_color.a == value) return;
		utils::scoped_lock guard(m_lock);
		m_color.a = value;
		invalidate();
	}
	
	
	void GObject::set_transform_point(const position_t& value)
	{
		if (m_transform_point == value) return;
		utils::scoped_lock guard(m_lock);
		m_transform_point = value;
		invalidate();
	}
	
	
	void GObject::set_rotation(const rotation_t& value)
	{
		if (m_rotation == value) return;
		utils::scoped_lock guard(m_lock);
		m_rotation = value;
		invalidate();
	}
	
	
	void GObject::set_scale(const scale_t& value)
	{
		if (m_scale == value) return;
		utils::scoped_lock guard(m_lock);
		m_scale = value;
		invalidate();
	}
	
	
	void GObject::draw()
	{
		utils::scoped_lock guard(m_lock);
		// Невидимые объекты не отображаются
		if (!m_visible) return;
		// Запоминаем текущую матрицу, она будет трансформироваться
		matrix().push();
		// Действия перед отрисовкой
		before_draw();
		// Определяем шейдерную программу
		set_program();
		// Рисуем объект
		do_draw();
		// Действия после отрисовки
		after_draw();
		// Восстанавливаем матрицу, удаляя все текущие трансформации
		matrix().pop();
		// Отключаем буферы от шейдерной программы
		if (m_program)
			m_program->reset_attributes();
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
		m_program->set_uniform_mvpmatrix(matrix().get());
		m_program->set_uniform_color(m_color);
	}
	
	
	void GObject::after_draw()
	{
	}
	
	
	void GObject::invalidate()
	{
		if (m_owner) m_owner->graphics_updated();
	}
	
}