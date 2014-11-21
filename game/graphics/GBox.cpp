#include <graphics/GBox.hpp>
#include <graphics/MatrixStack.hpp>
#include <utils/deleter.hpp>


namespace graphics
{
	
	GBox::GBox(const box_t &box)
	: super()
	, m_box(box)
	{
	}
	
	
	GBox::~GBox()
	{
	}
	
	
	void GBox::set_box(const box_t& value)
	{
		if (m_box == value) return;
		utils::scoped_lock guard(m_lock);
		m_box = value;
		invalidate_vertices();
		invalidate();
	}
	
	
	void GBox::update_vertices()
	{
		{
			m_vertices_count = 8 * 3;
			if (!m_vertices)
				m_vertices.reset(new GLfloat[m_vertices_count], utils::array_deleter<GLfloat>);
		}
		
		GLfloat *cursor = m_vertices.get();
		
		*cursor++ = 0.0f;
		*cursor++ = 0.0f;
		*cursor++ = 0.0f;
		
		*cursor++ = m_box.w;
		*cursor++ = 0.0f;
		*cursor++ = 0.0f;
		
		*cursor++ = m_box.w;
		*cursor++ = m_box.h;
		*cursor++ = 0.0f;
		
		*cursor++ = 0.0f;
		*cursor++ = m_box.h;
		*cursor++ = 0.0f;
		
		*cursor++ = 0.0f;
		*cursor++ = 0.0f;
		*cursor++ = m_box.d;
		
		*cursor++ = m_box.w;
		*cursor++ = 0.0f;
		*cursor++ = m_box.d;
		
		*cursor++ = m_box.w;
		*cursor++ = m_box.h;
		*cursor++ = m_box.d;
		
		*cursor++ = 0.0f;
		*cursor++ = m_box.h;
		*cursor++ = m_box.d;
		
		super::update_vertices();
	}
	
	
	void GBox::update_indices()
	{
		{
			m_indices_count = 6;
			if (!m_indices)
				m_indices.reset(new GLushort[m_indices_count], utils::array_deleter<GLushort>);
		}
		
		GLushort *cursor = m_indices.get();
		
		*cursor++ = 3;
		*cursor++ = 2;
		*cursor++ = 0;
		*cursor++ = 1;
		
		*cursor++ = 4;
		*cursor++ = 5;
		
		super::update_indices();
	}
	
	
	void GBox::before_draw()
	{
		// ВНИМАНИЕ! Трансформации выполняем в обратном порядке (от матрицы проекции к элементу меша), т.е. следующим
		// образом: Proj_m * View_m * Model_m (= Cur_m * Translate_m * Scale_m * Rot_m) * Element_v = Transformed_Element_v
		
		// Смещаем объект после поворота и масштабирования в конечную позицию объекта
		matrix().translate(m_box);
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
	
	
	void GBox::do_draw()
	{
		glDrawElements(GL_TRIANGLE_STRIP, m_indices_count, GL_UNSIGNED_SHORT, m_indices.get());
	}
	
}
