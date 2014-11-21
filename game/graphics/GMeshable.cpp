#include <graphics/GMeshable.hpp>
#include <graphics/ShaderProgram.hpp>


namespace graphics
{
	
	GMeshable::GMeshable(mesh_type_e mesh_type)
	: super()
	, m_mesh_type(mesh_type)
	, m_vertices_count(0)
	, m_indices_count(0)
	, m_vertices_valid(false)
	, m_indices_valid(false)
	{

	}
	
	
	GMeshable::~GMeshable()
	{
	}
	
	
	void GMeshable::update_vertices()
	{
		m_vertices_valid = true;
	}
	
	
	void GMeshable::update_indices()
	{
		m_indices_valid = true;
	}
	
	
	void GMeshable::before_draw()
	{
		// Обновляем графические данные, если они не актуальны
		if (!m_vertices_valid) update_vertices();
		if (!m_indices_valid) update_indices();
		
		super::before_draw();
	}
	
	
	void GMeshable::set_program()
	{
		super::set_program();
		switch (m_mesh_type) {
			case mesh_type_e::mt_2d:
				m_program->set_attribute_position2(m_vertices.get());
				break;
			case mesh_type_e::mt_3d:
				m_program->set_attribute_position3(m_vertices.get());
				break;
			default:
				throw("Некорректный тип вершинной сетки");
		}
	}
	
}