#include <graphics/GImages.hpp>
#include <graphics/MatrixStack.hpp>
#include <graphics/ShaderProgram.hpp>
#include <graphics/Texture.hpp>
#include <graphics/Context.hpp>
#include <utils/deleter.hpp>


namespace graphics
{
	
	GImages::GImages(const position_t& position)
	: super(mesh_type_e::mt_2d)
	, m_position(position)
	, m_img_coords_count(0)
	, m_msk_coords_count(0)
	{
	}
	
	
	GImages::~GImages()
	{
	}
	
	
	void GImages::set_position(const position_t& value)
	{
		if (m_position == value) return;
		utils::scoped_lock guard(m_lock);
		m_position = value;
		invalidate();
	}
	
	
	void GImages::add_region(const graphics::region_t& region)
	{
		m_regions.push_back(region);
		invalidate_img_coords();
		invalidate_msk_coords();
		invalidate_indexes();
		invalidate_vertices();
		invalidate();
	}
	
	
	void GImages::clear_regions()
	{
		m_regions.clear();
		invalidate_img_coords();
		invalidate_msk_coords();
		invalidate_indexes();
		invalidate_vertices();
		invalidate();
	}
	
	
	void GImages::set_image(const graphics::TextureSPtr &texture)
	{
		m_tex_img = texture;
		invalidate_img_coords();
		invalidate();
	}
	
	
	void GImages::set_mask(const graphics::TextureSPtr &texture)
	{
		m_tex_msk = texture;
		invalidate_msk_coords();
		invalidate();
	}
	
	
	void GImages::update_img_coords()
	{
		const GLsizei new_img_coords_count = 8 * m_regions.size();
		if (m_img_coords_count != new_img_coords_count)
		{
			m_img_coords_count = new_img_coords_count;
			if (m_img_coords_count)
			{
				m_img_coords.reset(new GLfloat[m_img_coords_count], utils::array_deleter<GLfloat>);
			}
			else
			{
				m_img_coords.reset();
				m_img_coords_valid = true;
				return;
			}
		}
		
		GLfloat *cursor = m_img_coords.get();
		GLfloat dw = 1.0 / m_tex_img->dimension().w;
		GLfloat dh = 1.0 / m_tex_img->dimension().h;
		for (auto it = m_regions.begin(), end = m_regions.end(); it != end; ++it)
		{
			const rectangle_t& rect = it->img_rect;
			*cursor++ = rect.x*dw;
			*cursor++ = (rect.y + rect.h)*dh;
			
			*cursor++ = (rect.x + rect.w)*dw;
			*cursor++ = (rect.y + rect.h)*dh;
			
			*cursor++ = (rect.x + rect.w)*dw;
			*cursor++ = rect.y*dh;
			
			*cursor++ = rect.x*dw;
			*cursor++ = rect.y*dh;
		}
		
		m_img_coords_valid = true;
	}
	
	
	void GImages::update_msk_coords()
	{
		const GLsizei new_msk_coords_count = 8 * m_regions.size();
		if (m_msk_coords_count != new_msk_coords_count)
		{
			m_msk_coords_count = new_msk_coords_count;
			if (m_msk_coords_count)
			{ 
				m_msk_coords.reset(new GLfloat[m_msk_coords_count], utils::array_deleter<GLfloat>);
			}
			else
			{
				m_msk_coords.reset();
				m_msk_coords_valid = true;
				return;
			}
		}
		GLfloat *cursor = m_msk_coords.get();
		GLfloat dw = 1.0 / m_tex_msk->dimension().w;
		GLfloat dh = 1.0 / m_tex_msk->dimension().h;
		for (auto it = m_regions.begin(), end = m_regions.end(); it != end; ++it)
		{
			const rectangle_t& rect = it->msk_rect;
			*cursor++ = rect.x*dw;
			*cursor++ = (rect.y + rect.h)*dh;
			
			*cursor++ = (rect.x + rect.w)*dw;
			*cursor++ = (rect.y + rect.h)*dh;
			
			*cursor++ = (rect.x + rect.w)*dw;
			*cursor++ = rect.y*dh;
			
			*cursor++ = rect.x*dw;
			*cursor++ = rect.y*dh;
		}
		
		m_msk_coords_valid = true;
	}
	
	
	void GImages::update_vertices()
	{
		const GLsizei new_vertices_count = 8 * m_regions.size();
		if (m_vertices_count != new_vertices_count)
		{
			m_vertices_count = new_vertices_count;
			if (m_vertices_count) {
				m_vertices.reset(new GLfloat[m_vertices_count], utils::array_deleter<GLfloat>);
			} else {
				m_vertices.reset();
				super::update_vertices();
				return;
			}
		}
		GLfloat *cursor = m_vertices.get();
		for (auto it = m_regions.begin(), end = m_regions.end(); it != end; ++it) {
			const rectangle_t& rect = it->rect;
			
			*cursor++ = rect.x;
			*cursor++ = rect.y;
			
			*cursor++ = rect.x + rect.w;
			*cursor++ = rect.y;
			
			*cursor++ = rect.x + rect.w;
			*cursor++ = rect.y + rect.h;
			
			*cursor++ = rect.x;
			*cursor++ = rect.y + rect.h;
		}
		
		super::update_vertices();
	}
	
	
	void GImages::update_indices()
	{ 
		const GLsizei new_indices_count = 6 * m_regions.size();
		if (m_indices_count != new_indices_count)
		{
			m_indices_count = new_indices_count;
			if (m_indices_count) {
				m_indices.reset(new GLushort[m_indices_count], utils::array_deleter<GLushort>);
			} else {
				m_indices.reset();
				super::update_indices();
				return;
			}
		}
		GLushort *cursor = m_indices.get();
		for (int i = 0, end = m_regions.size(); i != end; ++i)
		{
			*cursor++ = 0 + i * 4;
			*cursor++ = 1 + i * 4;
			*cursor++ = 2 + i * 4;
			*cursor++ = 0 + i * 4;
			*cursor++ = 2 + i * 4;
			*cursor++ = 3 + i * 4;
		}
		
		super::update_indices();
	}
	
	
	void GImages::before_draw()
	{
		// ВНИМАНИЕ! Трансформации выполняем в обратном порядке (от матрицы проекции к элементу меша), т.е. следующим
		// образом: Proj_m * View_m * Model_m (= Cur_m * Translate_m * Scale_m * Rot_m) * Element_v = Transformed_Element_v
		
		// Смещаем объект после поворота и масштабирования в конечную позицию объекта
		matrix().translate(m_position);
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
	
	
	void GImages::set_program()
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
	
	
	void GImages::do_draw()
	{
		if (m_tex_img) m_tex_img->use();
		if (m_tex_msk) m_tex_msk->use();
		ctx().draw_elements(GL_TRIANGLES, m_indices_count, GL_UNSIGNED_SHORT, m_indices.get());
	}
	
}
