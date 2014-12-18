#include <graphics/GRectangle.hpp>
#include <graphics/MatrixStack.hpp>
#include <graphics/Context.hpp>
#include <utils/deleter.hpp>


namespace graphics
{
	
	GRectangle::GRectangle(const rectangle_t &rect)
	: super(mesh_type_e::mt_2d)
	, m_rect(rect)
	{
	}
	
	
	GRectangle::~GRectangle()
	{
	}
	
	
	void GRectangle::set_rectangle(const rectangle_t& value)
	{
		if (m_rect == value) return;
		utils::scoped_lock guard(m_lock);
		m_rect = value;
		invalidate_vertices();
		invalidate();
	}
	
	
	void GRectangle::update_vertices()
	{
		{
			m_vertices_count = 4 * 2;
			if (!m_vertices)
				m_vertices.reset(new GLfloat[m_vertices_count], utils::array_deleter<GLfloat>);
		}
		
		GLfloat *cursor = m_vertices.get();
		
		*cursor++ = 0.0f;
		*cursor++ = 0.0f;
		
		*cursor++ = m_rect.w;
		*cursor++ = 0.0f;
		
		*cursor++ = m_rect.w;
		*cursor++ = m_rect.h;
		
		*cursor++ = 0.0f;
		*cursor++ = m_rect.h;
		
		super::update_vertices();
	}
	
	
	void GRectangle::update_indices()
	{
		{
			m_indices_count = 4;
			if (!m_indices)
				m_indices.reset(new GLushort[m_indices_count], utils::array_deleter<GLushort>);
		}
		
		GLushort *cursor = m_indices.get();
		
		*cursor++ = 0;
		*cursor++ = 1;
		*cursor++ = 3;
		*cursor++ = 2;
		
		super::update_indices();
	}
	
	
	void GRectangle::before_draw()
	{
		// ВНИМАНИЕ! Трансформации выполняем в обратном порядке (от матрицы проекции к элементу меша), т.е. следующим
		// образом: Proj_m * View_m * Model_m (= Cur_m * Translate_m * Scale_m * Rot_m) * Element_v = Transformed_Element_v
		
		// Смещаем объект после поворота и масштабирования в конечную позицию объекта
		matrix().translate(m_rect);
		// Возвращаем объект из точки трансформации в начальную позицию
		if (!!m_transform_point) matrix().translate(m_transform_point);
		// Масштабируем объект
		matrix().scale(scale());
		// Попорачиваем объект
		matrix().rotate(rotation());
		// Смещаем объект в точку трансформации
		if (!!m_transform_point) matrix().translate(-m_transform_point.x, -m_transform_point.y, -m_transform_point.z);
		
		super::before_draw();
	}
	
	
	void GRectangle::do_draw()
	{
		ctx().draw_elements(GL_TRIANGLE_STRIP, m_indices_count, GL_UNSIGNED_SHORT, m_indices.get());
	}
	
}