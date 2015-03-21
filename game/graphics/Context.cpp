#include <graphics/Context.hpp>
#include <graphics/Renderer.hpp>
#include <graphics/TextureManager.hpp>
#include <utils/log.hpp>
#include <utils/split.hpp>
#include <utils/libgen.hpp>
#include <utils/to_string.hpp>
#include <algorithm>
#include <set>

#define check_gl_err() graphics::show_gl_err(__FILE__, __LINE__)


namespace graphics
{
	
	const char* err_str(GLenum errorCode)
	{
		static const struct {
			GLenum code;
			const char *string;
		} errors[]=
		{
			{GL_NO_ERROR, "no error"},
			{GL_INVALID_ENUM, "invalid enumerant"},
			{GL_INVALID_VALUE, "invalid value"},
			{GL_INVALID_OPERATION, "invalid operation"},
			{GL_INVALID_FRAMEBUFFER_OPERATION, "invalid framebuffer operation"},
// 			{GL_STACK_OVERFLOW, "stack overflow"},
// 			{GL_STACK_UNDERFLOW, "stack underflow"},
			{GL_OUT_OF_MEMORY, "out of memory"},
			{0, NULL }
		};
		
		for (int i = 0; errors[i].string; ++i) {
			if (errors[i].code == errorCode) {
				return errors[i].string;
			}
		}
		
		return NULL;
	}
	
	int show_gl_err(const char *file, int line)
	{
		GLenum gl_err;
		int ret_code = 0;
		
		gl_err = glGetError();
		if (gl_err != GL_NO_ERROR) {
			LOG(ERR) << "glError in file " << (file ? utils::base_name(file) : "unknown") <<
				" @ line " << line << ": " << err_str(gl_err);
			ret_code = 1;
		}
		return ret_code;
	}
	
	
	struct Context::internal
	{
	private:
		typedef std::map<GLenum, GLuint> tex_bindings_t;
		
	public:
		internal();
		~internal();
		
		/** Установить флаги особых расширений в зависимости от наличия того или иного расширения в extensions
		 */
		void setup_extensions_flags();
		
	public:
		color_t clear_color;				///< Текущий цвет очистки рендербуфера
		color_t color;						///< Текущий цвет графических объектов
		unsigned char color_mask;			///< маска применения цвета к фрэймбуферу
		double depth_clear_value;			///< 
		GLenum depth_clear_func;			///< 
		GLenum blend_func[2];				///< 
		GLenum active_texture;				///< номер текущей активной текстуры GL_TEXTURE0, GL_TEXTURE1...
		
		rectangle_t scissor_box;			///< область отсечения рендеринга относительно экрана
		
		std::set<std::string> extensions;	///< Cписок особых расширений
		std::string vendor_string;			///< 
		std::string renderer_string;		///< 
		std::string version_string;			///< 
		
		bool has_ext_texture_npot;			///< Флаг особых расширений texture_npot
		bool has_ext_rgb8;					///< 
		
		std::map<GLenum, GLboolean> server_caps;			///< 
		std::vector<bool> vertex_attrib_arrays_states;		///< Состояния привязок буферов
		tex_bindings_t tex_bindings;						///< Привязки текстур к текстурным юнитам
	};
	
	
	Context::internal::internal()
	: clear_color()
	, color()
	, depth_clear_value(0.0)
	, depth_clear_func(GL_LESS)
	, active_texture(0)
	, extensions()
	, has_ext_texture_npot(false)
	, has_ext_rgb8(false)
	, server_caps()
	{
		GLfloat farr[4];
		
		{
			glGetFloatv(GL_COLOR_CLEAR_VALUE, farr);
			check_gl_err();
			clear_color.r = farr[0];
			clear_color.g = farr[1];
			clear_color.b = farr[2];
			clear_color.a = farr[3];
		}
		{
			glGetFloatv(GL_SCISSOR_BOX, farr);
			check_gl_err();
			scissor_box.x = farr[0];
			scissor_box.y = farr[1];
			scissor_box.w = farr[2];
			scissor_box.h = farr[3];
		}
		{
			const GLenum gl_caps[] = {
				GL_CULL_FACE,
				GL_BLEND,
				GL_DITHER,
				GL_STENCIL_TEST,
				GL_DEPTH_TEST,
				GL_SCISSOR_TEST,
				GL_POLYGON_OFFSET_FILL,
				GL_SAMPLE_ALPHA_TO_COVERAGE,
				GL_SAMPLE_COVERAGE
			};
			const int gl_caps_count = sizeof(gl_caps)/sizeof(GLenum);
			for (int i = 0; i < gl_caps_count; ++i)
			{
				server_caps[gl_caps[i]] = glIsEnabled(gl_caps[i]);
				check_gl_err();
			}
		}
		{
			glGetIntegerv(GL_BLEND_SRC_RGB, reinterpret_cast<GLint *>(&blend_func[0]));
			check_gl_err();
			glGetIntegerv(GL_BLEND_DST_RGB, reinterpret_cast<GLint *>(&blend_func[1]));
			check_gl_err();
		}
		{
			const GLubyte *vend = glGetString(GL_VENDOR);
			check_gl_err();
			if (vend) {
				vendor_string = reinterpret_cast<const char *>(vend);
				LOG(INFO) << "Vendor: " << vendor_string;
			}
			const GLubyte *rend = glGetString(GL_RENDERER);
			check_gl_err();
			if (rend) {
				renderer_string = reinterpret_cast<const char *>(rend);
				LOG(INFO) << "Renderer: " << renderer_string;
			}
			const GLubyte *vers = glGetString(GL_VERSION);
			check_gl_err();
			if (vers) {
				version_string = reinterpret_cast<const char *>(vers);
				LOG(INFO) << "Version: " << version_string;
			}
			const GLubyte *exts = glGetString(GL_EXTENSIONS);
			check_gl_err();
			if (exts) {
				std::string extensions_string(reinterpret_cast<const char *>(exts));
				std::transform(extensions_string.begin(), extensions_string.end(), extensions_string.begin(), toupper);
				LOG(INFO) << "Supported OpenGL extensions: " << extensions_string;
				utils::split(extensions, extensions_string, ' ');
				setup_extensions_flags();
			}
			const GLubyte *glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
			check_gl_err();
			if (glsl_version) {
				LOG(INFO) << "GLSL version: " << glsl_version;
			}
		}
	}
	
	
	Context::internal::~internal()
	{
	}
	
	
	void Context::internal::setup_extensions_flags()
	{
		// расширения GL GL_ARB_TEXTURE_NON_POWER_OF_TWO и GLES GL_OES_TEXTURE_NPOT полностью покрывают требования фреймворка,
		// поэтому можно их считать одинаковыми
		has_ext_texture_npot = extensions.count("GL_ARB_TEXTURE_NON_POWER_OF_TWO") || extensions.count("GL_OES_TEXTURE_NPOT");
		// проверка, поддерживается ли расширение GL_OES_rgb8_rgba8 - для полноцветного рендербуфера
		has_ext_rgb8 = extensions.count("GL_OES_RGB8_RGBA8") > 0;
	}
	
	
	
	/** \class gl */
	
	ContextSPtr gl::m_ctx(new ContextStub());
	rectangle_t gl::m_viewport;
	dimension_t gl::m_screen_size;
	bool gl::m_is_tablet = false;
	
	void gl::create()
	{
		if (m_ctx->is_active()) return;
		LOG(INFO) << "GL-context created";
		// Создаем активный gl-контекст и делаем его текущим
		m_ctx.reset(new Context());
		// Определяем начальное состояние графики
		m_ctx->enable_server_state(GL_BLEND);
		m_ctx->set_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_ctx->set_clear_color(color_t(0.0f, 0.0f, 0.0f, 0.0f));
		// Пересоздаем шейдеры и заново линкуем шейдерные программы
		renderer().load_shaders();
		// Восстанавливаем текстуры
		if (texmgr().is_alive()) texmgr().reload();
	}
	
	
	void gl::destroy()
	{
		if (!m_ctx->is_active()) return;
		LOG(INFO) << "GL-context destroyed";
		// Разрушаем шейдеры и удаляем шейдерные программы
		renderer().release_shaders();
		// Разрушаем текстуры
		if (texmgr().is_alive()) texmgr().unload();
		// Создаем неактивный gl-контекст и делаем его текущим
		m_ctx.reset(new ContextStub());
	}
	
	
	ContextBase* gl::ctx()
	{
		return m_ctx.get();
	}
	
	
	void gl::set_viewport(const rectangle_t& value, bool force)
	{
		if (value == m_viewport) {
			if (force) m_ctx->set_viewport(m_viewport);
			return;
		}
		m_viewport = value;
		m_ctx->set_viewport(m_viewport);
	}
	
	
	rectangle_t gl::get_viewport()
	{
		return m_viewport;
	}
	
	
	void gl::set_tablet(bool value)
	{
		m_is_tablet = value;
	}
	
	
	bool gl::get_tablet()
	{
		return m_is_tablet;
	}
	
	
	bool gl::set_screen_size(const dimension_t& value)
	{
		if (m_screen_size == value) return false;
		m_screen_size = value;
		return true;
	}
	
	
	dimension_t gl::get_screen_size()
	{
		return m_screen_size;
	}
	
	
	
	/** \class ContextBase */
	
	ContextBase::ContextBase()
	{
	}
	
	
	ContextBase::~ContextBase()
	{
	}
	
	
	
	/** \class Context */
	
	Context::Context()
	: m(new internal)
	{
		GLint max_vertex_attribs = 16;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vertex_attribs);
		check_gl_err();
		m->vertex_attrib_arrays_states.resize(max_vertex_attribs, false);
	}
	
	
	void Context::set_clear_color(const color_t& value)
	{
		if (m->clear_color == value) return;
		m->clear_color = value;
		glClearColor(m->clear_color.r, m->clear_color.g, m->clear_color.b, m->clear_color.a);
		check_gl_err();
	}
	
	
	color_t Context::get_clear_color() const
	{
		return m->clear_color;
	}
	
	
	void Context::set_depth_value(GLfloat value)
	{
		if (value == m->depth_clear_value) return;
		glClearDepthf(value);
		check_gl_err();
		m->depth_clear_value = value;
	}
	
	
	void Context::set_depth_function(GLenum value)
	{
		if (value == m->depth_clear_func) return;
		glDepthFunc(value);
		check_gl_err();
		m->depth_clear_func = value;
	}
	
	
	void Context::clear_buffers(GLbitfield mask)
	{
		glClear(mask);
		check_gl_err();
	}
	
	
	void Context::enable_server_state(const GLenum cap)
	{
		if (m->server_caps.count(cap) > 0) {
			if (m->server_caps[cap]) return;
		}
		glEnable(cap);
		check_gl_err();
		m->server_caps[cap] = GL_TRUE;
	}
	
	
	void Context::disable_server_state(const GLenum cap)
	{
		if (m->server_caps.count(cap) > 0) {
			if (!m->server_caps[cap]) return;
		}
		glDisable(cap);
		check_gl_err();
		m->server_caps[cap] = GL_FALSE;
	}
	
	
	void Context::enable_vertex_attr_array(GLuint index)
	{
		if (index < m->vertex_attrib_arrays_states.size()) {
			if (m->vertex_attrib_arrays_states[index]) return;
			m->vertex_attrib_arrays_states[index] = true;
		}
		glEnableVertexAttribArray(index);
		check_gl_err();
	}
	
	
	void Context::disable_vertex_attr_array(GLuint index)
	{
		if (index < m->vertex_attrib_arrays_states.size()) {
			if (!m->vertex_attrib_arrays_states[index]) return;
			m->vertex_attrib_arrays_states[index] = false;
		}
		glDisableVertexAttribArray(index);
		check_gl_err();
	}
	
	
	void Context::set_blend_func(const GLenum sfactor, const GLenum dfactor)
	{
		GLenum * const value = m->blend_func;
		if (sfactor == value[0] && dfactor == value[1]) return;
		glBlendFunc(sfactor, dfactor);
		check_gl_err();
		value[0] = sfactor;
		value[1] = dfactor;
	}
	
	
	void Context::draw_elements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
	{
		glDrawElements(mode, count, type, indices);
		check_gl_err();
	}
	
	
	GLuint Context::create_shader(GLenum type, const std::string& source)
	{
		// Создаем шейдер
		GLuint shader_id = 0;
		shader_id = glCreateShader(type);
		check_gl_err();
		ASSERT(shader_id);
		
		// Компилируем шейдер
		const GLint src_length = source.length();
		const char *source_text = source.c_str();
		glShaderSource(shader_id, 1, (const GLchar**)&source_text, &src_length);
		check_gl_err();
		glCompileShader(shader_id);
		check_gl_err();
		
		// Проверяем, что шейдер скомпилировался нормально
		{
			GLint status = 0, length = 0;
			GLchar buffer[1024];
			// Получаем статус шейдера
			glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
			check_gl_err();
			if (status != GL_TRUE) {
				// Ошибка компиляции
				glGetShaderInfoLog(shader_id, 1024, &length, buffer);
				check_gl_err();
				LOG(ERR) << "Ошибка при создании шейдера OpenGL: " << (const char*)buffer;
				LOG(ERR) << "Тип: " << shader_type(type) << "; Исходный текст шейдера:\n" << source;
			}
			// Если компиляция безуспешна, прекращаем работу программы
			CHECK(status == GL_TRUE);
		}
		
		return shader_id;
	}
	
	
	GLuint Context::create_program(GLuint vertex_shader, GLuint fragment_shader)
	{
		// Создаём программу
		GLuint program_id = 0;
		program_id = glCreateProgram();
		check_gl_err();
		ASSERT(program_id);
		
		// Указываем шейдеры программе
		glAttachShader(program_id, vertex_shader);
		check_gl_err();
		glAttachShader(program_id, fragment_shader);
		check_gl_err();
		
		// Линкуем и проверяем результат линковки
		glLinkProgram(program_id);
		check_gl_err();
		LOG_IF(FATAL, check_program_status(program_id, GL_LINK_STATUS) != GL_TRUE) << "Не удалось слинковать шейдерную программу";
		
		// Проверяем статус программы
		glValidateProgram(program_id);
		check_gl_err();
		LOG_IF(FATAL, check_program_status(program_id, GL_VALIDATE_STATUS) != GL_TRUE) << "Шейдерная программа не корректна";
		
		return program_id;
	}
	
	
	void Context::use_program(GLuint program_id)
	{
		glUseProgram(program_id);
		check_gl_err();
	}
	
	
	void Context::detach_shader(GLuint program_id, GLuint shader_id)
	{
		glDetachShader(program_id, shader_id);
		check_gl_err();
	}
	
	
	void Context::delete_shader(GLuint shader_id)
	{
		glDeleteShader(shader_id);
		check_gl_err();
	}
	
	
	void Context::delete_program(GLuint program_id)
	{
		glDeleteProgram(program_id);
		check_gl_err();
	}
	
	
	GLint Context::get_uniform_location(GLuint program, const GLchar* name)
	{
		GLint ret = glGetUniformLocation(program, name);
		check_gl_err();
		return ret;
	}
	
	
	GLint Context::get_attrib_location(GLuint program, const GLchar* name)
	{
		GLint ret = glGetAttribLocation(program, name);
		check_gl_err();
		return ret;
	}
	
	
	void Context::set_uniform_matrix(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value, const unsigned char& dimension)
	{
		switch (dimension)
		{
			case 2:
				glUniformMatrix2fv(location, count, transpose, value);
				check_gl_err();
				break;
			case 3:
				glUniformMatrix3fv(location, count, transpose, value);
				check_gl_err();
				break;
			case 4:
				glUniformMatrix4fv(location, count, transpose, value);
				check_gl_err();
				break;
			default:
				LOG(ERR) << "Unsupported matrix dimension!";
		}
	}
	
	
	void Context::set_uniform_vector(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{
		glUniform4f(location, v0, v1, v2, v3);
		check_gl_err();
	}
	
	
	void Context::set_uniform_value(GLint location, GLint v0)
	{
		glUniform1i(location, v0);
		check_gl_err();
	}
	
	
	void Context::set_uniform_value(GLint location, GLfloat v0)
	{
		glUniform1f(location, v0);
		check_gl_err();
	}
	
	
	void Context::set_pointer_vertex_attr(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
	{
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
		check_gl_err();
	}
	
	
	void Context::gen_textures(GLsizei n, GLuint *textures)
	{
		glGenTextures(n, textures);
		check_gl_err();
	}
	
	
	void Context::del_textures(GLsizei n, const GLuint *textures)
	{
		glDeleteTextures(n, textures);
		check_gl_err();
	}
	
	
	void Context::bind_texture(GLuint texture, GLenum unit)
	{
		// Выбираем текстурный юнит, к которому будет привязана текстура
		select_texture(unit);
		// Если текстура уже привязана к данноме текстурному юниту, ничего не делаем
		if (m->tex_bindings[unit] == texture) return;
		// Запоминаем текущую привязку текстуры
		m->tex_bindings[unit] = texture;
		// Выполняем привязку текстуры
		glBindTexture(GL_TEXTURE_2D, texture);
		check_gl_err();
	}
	
	
	bool Context::is_texture_binded(GLuint texture, GLenum unit)
	{
		return m->tex_bindings[unit] == texture;
	}
	
	
	void Context::select_texture(GLenum unit)
	{
		// Если указанный текстурый юнит уже активен, ничего не делаем
		if (m->active_texture == unit) return;
		// Запоминаем текущий активный текстурный юнит
		m->active_texture = unit;
		// Назначаем активный текстурный юнит
		glActiveTexture(m->active_texture);
		check_gl_err();
	}
	
	
	void Context::set_tex_param(GLenum target, GLenum pname, GLint param)
	{
		glTexParameteri(target, pname, param);
		check_gl_err();
	}
	
	
	void Context::load_texture(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data)
	{
		glTexImage2D(target, level, internalFormat, width, height, border, format, type, data);
		check_gl_err();
	}
	
	
	void Context::set_viewport(const rectangle_t& value)
	{
		glViewport(static_cast<int>(value.x), static_cast<int>(value.y), static_cast<int>(value.w), static_cast<int>(value.h));
		check_gl_err();
	}
	
	
	GLint Context::check_program_status(GLuint program_id, GLenum param)
	{
		GLint status = 0, length = 0;
		glGetProgramiv(program_id, param, &status);
		check_gl_err();
		if (status != GL_TRUE)
		{
			GLchar buffer[1024];
			glGetProgramInfoLog(program_id, 1024, &length, buffer);
			check_gl_err();
			LOG(ERR) << "Ошибка шейдерной программы OpenGL: " << (const char*)buffer;
		}
		return status;
	}
	
	
	std::string Context::shader_type(GLenum type)
	{
		if (type == GL_VERTEX_SHADER) return "GL_VERTEX_SHADER";
		else if (type == GL_FRAGMENT_SHADER) return "GL_FRAGMENT_SHADER";
		else return "Unknown (" + utils::to_string(type) + ")";
	}
	
	
	Context::~Context()
	{
	}
	
}
