#include <graphics/GImage.hpp>
#include <graphics/MatrixStack.hpp>
#include <graphics/ShaderProgram.hpp>
#include <graphics/Texture.hpp>
#include <graphics/Context.hpp>
#include <utils/deleter.hpp>


namespace graphics
{
	
	GImage::GImage(const rectangle_t& rect)
	: super(mesh_type_e::mt_2d)
	, m_img_coords_count(0)
	, m_msk_coords_count(0)
	, m_rect(rect)
	{
	}
	
	
	GImage::~GImage()
	{
	}
	
	
	void GImage::set_rectangle(const rectangle_t& value)
	{
		if (m_rect == value) return;
		utils::scoped_lock guard(m_lock);
		m_rect = value;
		invalidate_vertices();
		invalidate();
	}
	
	
	void GImage::set_image(const graphics::TextureSPtr &texture)
	{
		m_tex_img = texture;
		invalidate_img_coords();
		invalidate();
	}
	
	
	void GImage::set_mask(const graphics::TextureSPtr &texture)
	{
		m_tex_msk = texture;
		invalidate_msk_coords();
		invalidate();
	}
	
	
	void GImage::update_img_coords()
	{
		{
			m_img_coords_count = 4 * 2;
			if (!m_img_coords)
				m_img_coords.reset(new GLfloat[m_img_coords_count], utils::array_deleter<GLfloat>);
		}
		
		GLfloat *cursor = m_img_coords.get();
		
		*cursor++ = 0.0f;
		*cursor++ = 1.0f;
		
		*cursor++ = 1.0f;
		*cursor++ = 1.0f;
		
		*cursor++ = 1.0f;
		*cursor++ = 0.0f;
		
		*cursor++ = 0.0f;
		*cursor++ = 0.0f;
		
		m_img_coords_valid = true;
	}
	
	
	void GImage::update_msk_coords()
	{
		{
			m_msk_coords_count = 4 * 2;
			if (!m_msk_coords)
				m_msk_coords.reset(new GLfloat[m_msk_coords_count], utils::array_deleter<GLfloat>);
		}
		
		GLfloat *cursor = m_msk_coords.get();
		
		*cursor++ = 0.0f;
		*cursor++ = 1.0f;
		
		*cursor++ = 1.0f;
		*cursor++ = 1.0f;
		
		*cursor++ = 1.0f;
		*cursor++ = 0.0f;
		
		*cursor++ = 0.0f;
		*cursor++ = 0.0f;
		
		m_msk_coords_valid = true;
	}
	
	
	void GImage::update_vertices()
	{
		{
			m_vertices_count = 4 * 2;
			if (!m_vertices)
				m_vertices.reset(new GLfloat[m_vertices_count], utils::array_deleter<GLfloat>);
		}
		
		GLfloat *cursor = m_vertices.get();
		
		*cursor++ = 0.0f;
		*cursor++ = m_rect.h;
		
		*cursor++ = m_rect.w;
		*cursor++ = m_rect.h;
		
		*cursor++ = m_rect.w;
		*cursor++ = 0.0f;
		
		*cursor++ = 0.0f;
		*cursor++ = 0.0f;
		
		super::update_vertices();
	}
	
	
	void GImage::update_indices()
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
	
	
	void GImage::before_draw()
	{
		// ВНИМАНИЕ! Трансформации выполняем в обратном порядке (от матрицы проекции к элементу меша), т.е. следующим
		// образом: Proj_m * View_m * Model_m (= Cur_m * Translate_m * Scale_m * Rot_m) * Element_v = Transformed_Element_v
		
		// Смещаем объект после поворота и масштабирования в конечную позицию объекта
		matrix().translate(m_rect);
		// Возвращаем объект из точки трансформации в начальную позицию
		if (!!m_transform_point) matrix().translate(m_transform_point);
		// Масштабируем объект
		matrix().scale(scale());
		// Поворачиваем объект
		matrix().rotate(rotation());
		// Смещаем объект в точку трансформации
		if (!!m_transform_point) matrix().translate(-m_transform_point.x, -m_transform_point.y, -m_transform_point.z);
		
		// Обновляем графические данные, если они не актуальны
		if (m_tex_img && !m_img_coords_valid) update_img_coords();
		if (m_tex_msk && !m_msk_coords_valid) update_msk_coords();
		
		super::before_draw();
	}
	
	
	void GImage::set_program()
	{
		if (m_tex_img) {
			if (m_tex_msk) {
				m_program = renderer().get_program(sp_masked_image);
			} else {
				m_program = renderer().get_program(sp_image);
			}
		}
		super::set_program();
		if (m_tex_img) {
			if (m_tex_msk) {
				m_program->set_uniform_mask(m_tex_msk->unit_index());
				m_program->set_attribute_masktexcoord(m_msk_coords.get());
			}
			m_program->set_uniform_texture(m_tex_img->unit_index());
			m_program->set_attribute_texcoord(m_img_coords.get());
		}
	}
	
	
	void GImage::do_draw()
	{
		if (m_tex_img) m_tex_img->use();
		if (m_tex_msk) m_tex_msk->use();
		ctx().draw_elements(GL_TRIANGLE_STRIP, m_indices_count, GL_UNSIGNED_SHORT, m_indices.get());
	}
	
}
