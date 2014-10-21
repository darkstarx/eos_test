#include <graphics/GMeshable.hpp>
#include <graphics/ShaderProgram.hpp>


namespace graphics
{
	
	GMeshable::GMeshable()
	: super()
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
		m_program->set_attribute_position(m_vertices.get());
	}
	
}