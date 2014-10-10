#include <graphics/Context.hpp>
#include <utils/log.hpp>


namespace graphics
{
	
	Context *Context::m_instance = NULL;
	
	
	void Context::create()
	{
		ASSERT(!m_instance);
		m_instance = new Context();
	}
	
	
	void Context::destroy()
	{
		ASSERT(m_instance);
		delete m_instance;
		m_instance = NULL;
	}
	
	
	void Context::enable_vertex_attrib_array(GLuint index)
	{
		if (index < m_vertex_attrib_arrays_states.size()) {
			if (m_vertex_attrib_arrays_states[index]) return;
			m_vertex_attrib_arrays_states[index] = true;
		}
		glEnableVertexAttribArray(index);
	}
	
	
	void Context::disable_vertex_attrib_array(GLuint index)
	{
		if (index < m_vertex_attrib_arrays_states.size()) {
			if (!m_vertex_attrib_arrays_states[index]) return;
			m_vertex_attrib_arrays_states[index] = false;
		}
		glDisableVertexAttribArray(index);
	}
	
	
	Context::Context()
	{
		GLint max_vertex_attribs = 16;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vertex_attribs);
		m_vertex_attrib_arrays_states.resize(max_vertex_attribs, false);
	}
	
	
	Context::~Context()
	{
	}
	
}
