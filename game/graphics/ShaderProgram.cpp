#include <graphics/ShaderProgram.hpp>
#include <graphics/Context.hpp>
#include <utils/log.hpp>


namespace graphics
{
	
	ShaderProgram::ShaderProgram(const std::string &vertex_src, const std::string &fragment_src)
	{
		// Создаем вершинный шейдер
		m_vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_src);
		// Создаем Фрагментный шейдер
		m_fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_src);
		
		// Создаём программу
		m_program = glCreateProgram();
		ASSERT(m_program);
		
		// Указываем шейдеры программе
		glAttachShader(m_program, m_vertex_shader);
		glAttachShader(m_program, m_fragment_shader);
		
		// Линкуем и проверяем результат линковки
		glLinkProgram(m_program);
		LOG_IF(FATAL, check_program_status(GL_LINK_STATUS) != GL_TRUE) << "Не удалось слинковать шейдерную программу";
		
		// Проверяем статус программы
		glValidateProgram(m_program);
		LOG_IF(FATAL, check_program_status(GL_VALIDATE_STATUS) != GL_TRUE) << "Шейдерная программа не корректна";
		
		// Идентификатор униформы u_mvpmatrix
		uniforms.u_mvpmatrix = glGetUniformLocation(m_program, "u_mvpmatrix");
		ASSERT(uniforms.u_mvpmatrix != INVALID_ID);
		
		// Идентификатор униформы u_color (может отсутствовать)
		uniforms.u_color = glGetUniformLocation(m_program, "u_color");
		
		// Идентификатор атрибута a_position
		attributes.a_position = glGetAttribLocation(m_program, "a_position");
		ASSERT(attributes.a_position != INVALID_ID);
	}
	
	
	ShaderProgram::~ShaderProgram()
	{
		glDetachShader(m_program, m_vertex_shader);
		glDetachShader(m_program, m_fragment_shader);
		glDeleteShader(m_vertex_shader);
		glDeleteShader(m_fragment_shader);
		glDeleteProgram(m_program);
	}
	
	
	void ShaderProgram::use()
	{
		glUseProgram(m_program);
	}
	
	
	void ShaderProgram::set_uniform(const GLfloat value, const std::string& name)
	{
		GLint uniform_ptr;
		uniform_ptr = glGetUniformLocation(m_program, name.c_str());
		if (uniform_ptr != INVALID_ID)
		{
			glUniform1f(uniform_ptr, value);
		}
	}
	
	
	void ShaderProgram::set_uniform_mvpmatrix(const glm::mat4& matrix)
	{
		if (matrix == uniform_values.u_mvpmatrix) return;
		uniform_values.u_mvpmatrix = matrix;
		use();
		glUniformMatrix4fv(uniforms.u_mvpmatrix, 1, GL_FALSE, &matrix[0][0]);
	}
	
	
	void ShaderProgram::set_uniform_color(const color_t& color)
	{
		if (color == uniform_values.u_color) return;
		uniform_values.u_color = color;
		use();
		ASSERT(uniforms.u_color != INVALID_ID);
		glUniform4f(uniforms.u_color, color.r, color.g, color.b, color.a);
	}
	
	
	void ShaderProgram::set_uniform_texture(GLint unit)
	{
		use();
		if (uniforms.u_texture == INVALID_ID)
		{
			uniforms.u_texture = glGetUniformLocation(m_program, "u_texture");
			ASSERT(uniforms.u_texture != INVALID_ID);
		}
		glUniform1i(uniforms.u_texture, unit);
	}
	
	
	void ShaderProgram::set_uniform_mask(GLint unit)
	{
		use();
		if (uniforms.u_mask == INVALID_ID)
		{
			uniforms.u_mask = glGetUniformLocation(m_program, "u_mask");
			ASSERT(uniforms.u_mask != INVALID_ID);
		}
		glUniform1i(uniforms.u_mask, unit);
	}
	
	
	void ShaderProgram::set_uniform_texmatrix(const glm::mat3& matrix)
	{
		if (matrix == uniform_values.u_texmatrix) return;
		uniform_values.u_texmatrix = matrix;
		use();
		if (uniforms.u_texmatrix == INVALID_ID)
		{
			uniforms.u_texmatrix = glGetUniformLocation(m_program, "u_texmatrix");
			ASSERT(uniforms.u_texmatrix != INVALID_ID);
		}
		glUniformMatrix3fv(uniforms.u_texmatrix, 1, GL_FALSE, &matrix[0][0]);
	}
	
	
	void ShaderProgram::set_attribute_position(const void* data, GLsizei stride)
	{
		
		ctx().enable_vertex_attrib_array(attributes.a_position);
		glVertexAttribPointer(attributes.a_position, 2, GL_FLOAT, GL_FALSE, stride, data);
	}
	
	
	void ShaderProgram::set_attribute_texcoord(const void* data, GLsizei stride)
	{
		if (attributes.a_texcoord == INVALID_ID)
		{
			attributes.a_texcoord = glGetAttribLocation(m_program, "a_texcoord");
			ASSERT(attributes.a_texcoord != INVALID_ID);
		}
		ctx().enable_vertex_attrib_array(attributes.a_texcoord);
		glVertexAttribPointer(attributes.a_texcoord, 2, GL_FLOAT, GL_FALSE, stride, data);
	}
	
	
	void ShaderProgram::set_attribute_masktexcoord(const void* data, GLsizei stride)
	{
		if (attributes.a_masktexcoord == INVALID_ID)
		{
			attributes.a_masktexcoord = glGetAttribLocation(m_program, "a_masktexcoord");
			ASSERT(attributes.a_masktexcoord != INVALID_ID);
		}
		ctx().enable_vertex_attrib_array(attributes.a_masktexcoord);
		glVertexAttribPointer(attributes.a_masktexcoord, 2, GL_FLOAT, GL_FALSE, stride, data);
	}
	
	
	void ShaderProgram::set_attribute_color(const void* data, GLsizei stride)
	{
		if (attributes.a_color == INVALID_ID)
		{
			attributes.a_color = glGetAttribLocation(m_program, "a_color");
			ASSERT(attributes.a_color != INVALID_ID);
		}
		ctx().enable_vertex_attrib_array(attributes.a_color);
		glVertexAttribPointer(attributes.a_color, 4, GL_FLOAT, GL_FALSE, stride, data);
	}
	
	
	void ShaderProgram::reset_attributes()
	{
		const GLint all_attributes[] = { attributes.a_color, attributes.a_masktexcoord, attributes.a_position, attributes.a_texcoord };
		for (size_t i = 0; i < sizeof(all_attributes)/sizeof(*all_attributes); ++i)
		{
			if (all_attributes[i] != INVALID_ID)
				ctx().disable_vertex_attrib_array(all_attributes[i]);
		}
	}
	
	
	GLuint ShaderProgram::create_shader(GLenum type, const std::string& source)
	{
		// Создаем шейдер
		GLuint shader = glCreateShader(type);
		ASSERT(shader);
		
		// Компилируем шейдер
		const GLint src_length = source.length();
		const char *source_text = source.c_str();
		glShaderSource(shader, 1, (const GLchar**)&source_text, &src_length);
		glCompileShader(shader);
		
		// Проверяем, что шейдер скомпилировался нормально
		{
			GLint status = 0, length = 0;
			GLchar buffer[1024];
			// Получаем статус шейдера
			glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
			if (status != GL_TRUE) {
				// Ошибка компиляции
				glGetShaderInfoLog(shader, 1024, &length, buffer);
				LOG(ERR) << "Ошибка при создании шейдера OpenGL: " << (const char*)buffer;
				LOG(ERR) << "Тип: " << type << "; Исходный текст шейдера: " << source;
			}
			// Если компиляция безуспешна, прекращаем работу программы
			CHECK(status == GL_TRUE);
		}
		
		return shader;
	}
	
	
	GLint ShaderProgram::check_program_status(GLenum param)
	{
		GLint status = 0, length = 0;
		GLchar buffer[1024];
		glGetProgramiv(m_program, param, &status);
		if (status != GL_TRUE) {
			glGetProgramInfoLog(m_program, 1024, &length, buffer);
			LOG(ERR) << "Ошибка шейдерной программы OpenGL: " << (const char*)buffer;
		}
		return status;
	}
	
}
