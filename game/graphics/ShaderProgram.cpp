#include <graphics/ShaderProgram.hpp>
#include <graphics/Context.hpp>
#include <utils/log.hpp>
#include <utils/to_string.hpp>


namespace graphics
{
	
	ShaderProgram::ShaderProgram(const std::string &vertex_src, const std::string &fragment_src)
	: m_vertex_shader(0)
	, m_fragment_shader(0)
	, m_program(0)
	{
		initialize(vertex_src, fragment_src);
	}
	
	
	ShaderProgram::~ShaderProgram()
	{
		deinitialize();
	}
	
	
	void ShaderProgram::initialize(const std::string& vertex_src, const std::string& fragment_src)
	{
		if (m_program) {
			LOG(WARN) << "Повторная инициализация проинициализированной шейдерной программы!";
			return;
		}
		
		// Создаем вершинный шейдер
		m_vertex_shader = ctx().create_shader(GL_VERTEX_SHADER, vertex_src);
		// Создаем Фрагментный шейдер
		m_fragment_shader = ctx().create_shader(GL_FRAGMENT_SHADER, fragment_src);
		
		// Создаём программу
		m_program = ctx().create_program(m_vertex_shader, m_fragment_shader);
		
		// Идентификатор униформы u_mvpmatrix
		uniforms.u_mvpmatrix = ctx().get_uniform_location(m_program, "u_mvpmatrix");
		ASSERT(uniforms.u_mvpmatrix != INVALID_ID);
		
		// Идентификатор униформы u_color (может отсутствовать)
		uniforms.u_color = ctx().get_uniform_location(m_program, "u_color");
		
		// Идентификатор атрибута a_position
		attributes.a_position = ctx().get_attrib_location(m_program, "a_position");
		ASSERT(attributes.a_position != INVALID_ID);
	}
	
	
	void ShaderProgram::deinitialize()
	{
		if (!m_program) {
			LOG(WARN) << "Деинициализация не инициализированной шейдерной программы!";
			return;
		}
		
		ctx().detach_shader(m_program, m_vertex_shader);
		ctx().detach_shader(m_program, m_fragment_shader);
		ctx().delete_shader(m_vertex_shader);
		ctx().delete_shader(m_fragment_shader);
		ctx().delete_program(m_program);
		
		m_vertex_shader = 0;
		m_fragment_shader = 0;
		m_program = 0;
	}
	
	
	void ShaderProgram::use()
	{
		if (!m_program) return;
		ctx().use_program(m_program);
	}
	
	
	void ShaderProgram::set_uniform(const GLfloat value, const std::string& name)
	{
		if (!m_program) return;
		GLint uniform;
		uniform = ctx().get_uniform_location(m_program, name.c_str());
		if (uniform != INVALID_ID)
		{
			ctx().set_uniform_value(uniform, value);
		}
	}
	
	
	void ShaderProgram::set_uniform_mvpmatrix(const glm::mat4& matrix)
	{
		if (!m_program) return;
		if (matrix == uniform_values.u_mvpmatrix) return;
		uniform_values.u_mvpmatrix = matrix;
		ctx().set_uniform_matrix(uniforms.u_mvpmatrix, 1, GL_FALSE, &matrix[0][0], 4);
	}
	
	
	void ShaderProgram::set_uniform_color(const color_t& color)
	{
		if (!m_program) return;
		if (color == uniform_values.u_color) return;
		uniform_values.u_color = color;
		ASSERT(uniforms.u_color != INVALID_ID);
		ctx().set_uniform_vector(uniforms.u_color, color.r, color.g, color.b, color.a);
	}
	
	
	void ShaderProgram::set_uniform_texture(GLint index)
	{
		if (!m_program) return;
		if (uniforms.u_texture == INVALID_ID)
		{
			uniforms.u_texture = ctx().get_uniform_location(m_program, "u_texture");
			ASSERT(uniforms.u_texture != INVALID_ID);
		}
		ctx().set_uniform_value(uniforms.u_texture, index);
	}
	
	
	void ShaderProgram::set_uniform_mask(GLint index)
	{
		if (!m_program) return;
		if (uniforms.u_mask == INVALID_ID)
		{
			uniforms.u_mask = ctx().get_uniform_location(m_program, "u_mask");
			ASSERT(uniforms.u_mask != INVALID_ID);
		}
		ctx().set_uniform_value(uniforms.u_mask, index);
	}
	
	
	void ShaderProgram::set_uniform_texmatrix(const glm::mat3& matrix)
	{
		if (!m_program) return;
		if (matrix == uniform_values.u_texmatrix) return;
		uniform_values.u_texmatrix = matrix;
		if (uniforms.u_texmatrix == INVALID_ID)
		{
			uniforms.u_texmatrix = ctx().get_uniform_location(m_program, "u_texmatrix");
			ASSERT(uniforms.u_texmatrix != INVALID_ID);
		}
		ctx().set_uniform_matrix(uniforms.u_texmatrix, 1, GL_FALSE, &matrix[0][0], 3);
	}
	
	
	void ShaderProgram::set_attribute_position2(const void* data, GLsizei stride)
	{
		if (!m_program) return;
		ctx().enable_vertex_attr_array(attributes.a_position);
		ctx().set_pointer_vertex_attr(attributes.a_position, 2, GL_FLOAT, GL_FALSE, stride, data);
	}
	
	
	void ShaderProgram::set_attribute_position3(const void* data, GLsizei stride)
	{
		if (!m_program) return;
		ctx().enable_vertex_attr_array(attributes.a_position);
		ctx().set_pointer_vertex_attr(attributes.a_position, 3, GL_FLOAT, GL_FALSE, stride, data);
	}
	
	
	void ShaderProgram::set_attribute_texcoord(const void* data, GLsizei stride)
	{
		if (!m_program) return;
		if (attributes.a_texcoord == INVALID_ID)
		{
			attributes.a_texcoord = ctx().get_attrib_location(m_program, "a_texcoord");
			ASSERT(attributes.a_texcoord != INVALID_ID);
		}
		ctx().enable_vertex_attr_array(attributes.a_texcoord);
		ctx().set_pointer_vertex_attr(attributes.a_texcoord, 2, GL_FLOAT, GL_FALSE, stride, data);
	}
	
	
	void ShaderProgram::set_attribute_masktexcoord(const void* data, GLsizei stride)
	{
		if (!m_program) return;
		if (attributes.a_masktexcoord == INVALID_ID)
		{
			attributes.a_masktexcoord = ctx().get_attrib_location(m_program, "a_masktexcoord");
			ASSERT(attributes.a_masktexcoord != INVALID_ID);
		}
		ctx().enable_vertex_attr_array(attributes.a_masktexcoord);
		ctx().set_pointer_vertex_attr(attributes.a_masktexcoord, 2, GL_FLOAT, GL_FALSE, stride, data);
	}
	
	
	void ShaderProgram::set_attribute_color(const void* data, GLsizei stride)
	{
		if (!m_program) return;
		if (attributes.a_color == INVALID_ID)
		{
			attributes.a_color = ctx().get_attrib_location(m_program, "a_color");
			ASSERT(attributes.a_color != INVALID_ID);
		}
		ctx().enable_vertex_attr_array(attributes.a_color);
		ctx().set_pointer_vertex_attr(attributes.a_color, 4, GL_FLOAT, GL_FALSE, stride, data);
	}
	
	
	void ShaderProgram::reset_attributes()
	{
		if (!m_program) return;
		const GLint all_attributes[] = { attributes.a_color, attributes.a_masktexcoord, attributes.a_position, attributes.a_texcoord };
		for (size_t i = 0; i < sizeof(all_attributes)/sizeof(*all_attributes); ++i)
		{
			if (all_attributes[i] != INVALID_ID)
				ctx().disable_vertex_attr_array(all_attributes[i]);
		}
	}
	
}
