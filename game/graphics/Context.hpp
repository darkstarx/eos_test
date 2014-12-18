#ifndef GRAPHICS_CONTEXT_HPP
#define GRAPHICS_CONTEXT_HPP

#include <graphics/gl.hpp>
#include <graphics/types.hpp>
#include <memory>
#include <vector>
#include <map>


namespace graphics
{
	
	/** \brief Интерфейс GL-контекста
	 */
	class ContextBase
	{
	private:
		friend class gl;
		
	public:
		ContextBase(const ContextBase&) = delete;
		
		ContextBase& operator=(const ContextBase&) = delete;
		
		/** \brief Получить флаг активности gl-контекста
		 * Если gl-контекст разрушен, он будет неактивным.
		 * \return true если активен, false если не активен.
		 */
		virtual bool is_active() = 0;
		
		/**
		 */
		virtual void set_clear_color(const color_t &value) = 0;
		
		/**
		 */
		virtual color_t get_clear_color() const = 0;
		
		/** Установить значение для очистки буфера глубины
		 * \param value Значение для очистки.
		 */
		virtual void set_depth_value(GLfloat value) = 0;
		
		/** Specify the value used for depth buffer comparisons
		 * \param value Specifies the depth comparison function. Symbolic constants GL_NEVER, GL_LESS, GL_EQUAL,
		 * GL_LEQUAL, GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, and GL_ALWAYS are accepted. The initial value is GL_LESS.
		 */
		virtual void set_depth_function(GLenum value) = 0;
		
		/** Clear buffers to preset values
		 */
		virtual void clear_buffers(GLbitfield mask) = 0;
		
		/** Активировать server-side GL-фичу
		 * \param cap - GL-фича
		 */
		virtual void enable_server_state(const GLenum cap) = 0;
		
		/** Отключить server-side GL-фичу
		 * \param cap - GL-фича
		 */
		virtual void disable_server_state(const GLenum cap) = 0;
		
		/** Enable a generic vertex attribute array
		 * \param index Specifies the index of the generic vertex attribute to be enabled.
		 */
		virtual void enable_vertex_attr_array(GLuint index) = 0;
		
		/** Disable a generic vertex attribute array
		 * \param index Specifies the index of the generic vertex attribute to be disabled.
		 */
		virtual void disable_vertex_attr_array(GLuint index) = 0;
		
		/** Установить параметры пиксельного смешивания
		 * \param sfactor метод выборки цветовых компонентов из источника
		 * \param dfactor метод выборки цветовых компонентов из приемника
		 */
		virtual void set_blend_func(const GLenum sfactor, const GLenum dfactor) = 0;
		
		/** Render primitives from array data
		 * \param mode Specifies what kind of primitives to render. Symbolic constants GL_POINTS,
		 * GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES,
		 * GL_QUAD_STRIP, GL_QUADS, and GL_POLYGON are accepted.
		 * \param count Specifies the number of elements to be rendered.
		 * \param type Specifies the type of the values in indices. Must be one of GL_UNSIGNED_BYTE,
		 * GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT.
		 * \param indices Specifies a pointer to the location where the indices are stored.
		 */
		virtual void draw_elements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices) = 0;
		
		/** Создать и скомпилировать шейдер
		 * \param type Тип шейдера (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER).
		 * \param source Код шейдера.
		 * \return Возвращает идентификатор шейдера.
		 */
		virtual GLuint create_shader(GLenum type, const std::string& source) = 0;
		
		/** Создать и слинковать шейдерную программу
		 * \param vertex_shader Вершинный шейдер.
		 * \param fragment_shader Пиксельный шейдер.
		 * \return Возвращает идентификатор программы.
		 */
		virtual GLuint create_program(GLuint vertex_shader, GLuint fragment_shader) = 0;
		
		/** Сделать шейдерную программу активной
		 * \param program_id Идентификатор программы.
		 */
		virtual void use_program(GLuint program_id) = 0;
		
		/** Отсоединить шейдер от программы
		 * \param shader_id Идентификатор шейдера.
		 * \param program_id Идентификатор программы.
		 */
		virtual void detach_shader(GLuint program_id, GLuint shader_id) = 0;
		
		/** Удалить шейдер
		 * \param shader_id Идентификатор шейдера.
		 */
		virtual void delete_shader(GLuint shader_id) = 0;
		
		/** Удалить шейдерную программу
		 * \param program_id Идентификатор программы.
		 */
		virtual void delete_program(GLuint program_id) = 0;
		
		/** Returns the location of a uniform variable
		 * \param program Specifies the program object to be queried.
		 * \param name Points to a null terminated string containing the name of the uniform variable whose location
		 *   is to be queried.
		 */
		virtual GLint get_uniform_location(GLuint program, const GLchar *name) = 0;
		
		/** Returns the location of an attribute variable
		 * \param rogram Specifies the program object to be queried.
		 * \param name Points to a null terminated string containing the name of the attribute variable whose location
		 *   is to be queried.
		 */
		virtual GLint get_attrib_location(GLuint program, const GLchar *name) = 0;
		
		/** Specify the value of a uniform matrix for the current program object
		 * \param location Specifies the location of the uniform value to be modified.
		 * \param count Specifies the number of matrices that are to be modified. This should be 1 if the targeted
		 *   uniform variable is not an array of matrices, and 1 or more if it is an array of matrices.
		 * \param transpose Specifies whether to transpose the matrix as the values are loaded into the uniform
		 *   variable. Must be GL_FALSE.
		 * \param value Specifies a pointer to an array of count values that will be used to update the specified uniform
		 *   variable.
		 */
		virtual void set_uniform_matrix(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value, const unsigned char &dimension) = 0;
		
		/** Specify the value of a uniform vector for the current program object
		 * \param location Specifies the location of the uniform variable to be modified.
		 * \param v0, v1, v2, v3 Specifies the new values to be used for the specified uniform variable.
		 */
		virtual void set_uniform_vector(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) = 0;
		
		/** Specify the value of a uniform variable for the current program object
		 * \param location Specifies the location of the uniform variable to be modified.
		 * \param v0 Specifies the new value to be used for the specified uniform variable.
		 */
		virtual void set_uniform_value(GLint location, GLint v0) = 0;
		
		/** Specify the value of a uniform variable for the current program object
		 * \param location Specifies the location of the uniform variable to be modified.
		 * \param v0 Specifies the new value to be used for the specified uniform variable.
		 */
		virtual void set_uniform_value(GLint location, GLfloat v0) = 0;
		
		/** Define an array of generic vertex attribute data
		 * \param index Specifies the index of the generic vertex attribute to be modified.sizeSpecifies the number
		 *   of components per generic vertex attribute. Must be 1, 2, 3, or 4. The initial value is 4.
		 * \param size Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4. Additionally,
		 *   the symbolic constant GL_BGRA is accepted by glVertexAttribPointer. The initial value is 4.
		 * \param type Specifies the data type of each component in the array. Symbolic constants GL_BYTE, GL_UNSIGNED_BYTE,
		 *   GL_SHORT, GL_UNSIGNED_SHORT, GL_FIXED, or GL_FLOAT are accepted. The initial value is GL_FLOAT.
		 * \param normalized Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly
		 *   as fixed-point values (GL_FALSE) when they are accessed.
		 * \param stride Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic
		 *   vertex attributes are understood to be tightly packed in the array. The initial value is 0.
		 * \param pointer Specifies a pointer to the first component of the first generic vertex attribute in the array.
		 *   The initial value is 0.
		 */
		virtual void set_pointer_vertex_attr(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) = 0;
		
		/** Generate texture names
		 * \param n Specifies the number of texture names to be generated.
		 * \param textures Specifies an array in which the generated texture names are stored.
		 */
		virtual void gen_textures(GLsizei n, GLuint *textures) = 0;
		
		/** Delete named textures
		 * \param n Specifies the number of textures to be deleted.
		 * \param textures Specifies an array of textures to be deleted.
		 */
		virtual void del_textures(GLsizei n, const GLuint *textures) = 0;
		
		/** Bind a named texture to a texturing target
		 * \param texture Specifies the name of a texture.
		 * \param unit Определяет, в каком текстурном юните будет выполнена привязка текстуры.
		 */
		virtual void bind_texture(GLuint texture, GLenum unit) = 0;
		
		/** Check if the texture is bind to a texturing target
		 * \param texture Specifies the name of a texture.
		 * \param unit Определяет, в каком текстурном юните будет выполнена привязка текстуры.
		 */
		virtual bool is_texture_binded(GLuint texture, GLenum unit) = 0;
		
		/** Select active texture unit
		 * \param unit Specifies which texture unit to make active. The number of texture units is implementation dependent,
		 * but must be at least 80. texture must be one of GL_TEXTUREi, where i ranges from zero to the value of
		 * GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS minus one. The initial value is GL_TEXTURE0.
		 */
		virtual void select_texture(GLenum unit) = 0;
		
		/** Set texture parameters
		 * \param target Specifies the target texture of the active texture unit, which must be either GL_TEXTURE_2D or GL_TEXTURE_CUBE_MAP.
		 * \param pname Specifies the symbolic name of a single-valued texture parameter. pname can be one of the following:
		 *   GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_TEXTURE_WRAP_S, or GL_TEXTURE_WRAP_T.
		 * \param param Specifies the value of pname.
		 */
		virtual void set_tex_param(GLenum target, GLenum pname, GLint param) = 0;
		
		/** Specify a two-dimensional texture image
		 * \param target Specifies the target texture. Must be GL_TEXTURE_2D, GL_PROXY_TEXTURE_2D, GL_TEXTURE_1D_ARRAY,
		 *   GL_PROXY_TEXTURE_1D_ARRAY, GL_TEXTURE_RECTANGLE, GL_PROXY_TEXTURE_RECTANGLE, GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		 *   GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		 *   GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, or GL_PROXY_TEXTURE_CUBE_MAP.
		 * \param level Specifies the level-of-detail number. Level 0 is the base image level. Level n is the nth mipmap reduction image.
		 *   If target is GL_TEXTURE_RECTANGLE or GL_PROXY_TEXTURE_RECTANGLE, level must be 0. 
		 * \param internalFormat Specifies the number of color components in the texture. Must be one of base internal formats given in Table 1,
		 *   one of the sized internal formats given in Table 2, or one of the compressed internal formats given in Table 3, below. 
		 * \param width Specifies the width of the texture image. All implementations support texture images that are at least 1024 texels wide. 
		 * \param height Specifies the height of the texture image, or the number of layers in a texture array, in the case of the
		 *   GL_TEXTURE_1D_ARRAY and GL_PROXY_TEXTURE_1D_ARRAY targets. All implementations support 2D texture images that are at least 1024
		 *   texels high, and texture arrays that are at least 256 layers deep. 
		 * \param border This value must be 0.
		 * \param format Specifies the format of the pixel data. The following symbolic values are accepted: GL_RED, GL_RG, GL_RGB, GL_BGR,
		 *   GL_RGBA, GL_BGRA, GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_BGR_INTEGER, GL_RGBA_INTEGER, GL_BGRA_INTEGER, GL_STENCIL_INDEX,
		 *   GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL.
		 * \param type Specifies the data type of the pixel data. The following symbolic values are accepted: GL_UNSIGNED_BYTE, GL_BYTE,
		 *   GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_FLOAT, GL_UNSIGNED_BYTE_3_3_2, GL_UNSIGNED_BYTE_2_3_3_REV,
		 *   GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_5_6_5_REV, GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_4_4_4_4_REV,
		 *   GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_UNSIGNED_INT_8_8_8_8, GL_UNSIGNED_INT_8_8_8_8_REV,
		 *   GL_UNSIGNED_INT_10_10_10_2, and GL_UNSIGNED_INT_2_10_10_10_REV.
		 * \param data Specifies a pointer to the image data in memory.
		 */
		virtual void load_texture(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border,
			GLenum format, GLenum type, const GLvoid *data) = 0;
		
		virtual ~ContextBase();
		
	protected:
		/** Установить ректангл вьюпорта для GL рендерера
		 * \param value ректангл вьюпорта
		 */
		virtual void set_viewport(const rectangle_t &value) = 0;
		
		ContextBase();
	};
	
	typedef std::shared_ptr<ContextBase> ContextSPtr;
	
	
	
	/** \brief Доступ к функциям OpenGL */
	class gl
	{
	public:
		/** \brief Выполнить действия при создании GL-контекста
		 */
		static void create();
		
		/** \brief Выполнить действия при разрушении GL-контекста
		 */
		static void destroy();
		
		/** \brief Получить указатель на GL-контекст для доступа к функциям OpenGL
		 */
		static ContextBase* ctx();
		
		/** Установить ректангл вьюпорта для GL рендерера
		 * \param value Ректангл вьюпорта.
		 * \param force Если true, то вьюпорт будет установлен даже если он совпадает с последним сохраненным.
		 */
		static void set_viewport(const rectangle_t &value, bool force = false);
		
		/** Получить ректангл вьюпорта для GL рендера
		 * Возвращает ректангл экрана, точнее поверхности GL (на мобильниках обычно соответствует разрешению экрана).
		 * \return Ректангл вьюпорта
		 */
		static rectangle_t get_viewport();
		
		/** Установить признак планшетного экрана
		 * \param value Признак того, что экран принадлежит планшетному устройству (экран большой)
		 */
		static void set_tablet(bool value);
		
		/** Получить признак планшетного экрана
		 * \return Признак планшетного экрана.
		 */
		static bool get_tablet();
		
		/** Установить физические размеры дисплея
		 * \param value Физические размеры дисплея.
		 * \return True если размеры изменились, иначе fals
		 */
		static bool set_screen_size(const dimension_t &value);
		
		/** Получить размеры дисплея в пикселах
		 * \return Размеры дисплея в пикселах.
		 */
		static dimension_t get_screen_size();
		
	private:
		/** Конструктор никогда не вызывается (pure static class)
		 */
		gl() {}
		
		/** Деструктор никогда не вызывается (pure static class)
		 */
		~gl() {}
		
		static ContextSPtr m_ctx;			///< контекст вызовов к gl api
		static rectangle_t m_viewport;		///< вьюпорт, установленный для GL рендера (т.к. мы скейлим рендер до разрешения физического экрана, размеры совпадают с "screen_dimention" )
		static dimension_t m_screen_size;	///< реальные размеры экрана
		static bool m_is_tablet;			///< признак планшетности
	};
	
	
	
	/** \brief Активный GL-контекст
	 */
	class Context : public ContextBase
	{
	private:
		friend class gl;
		
	public:
		virtual inline bool is_active() { return true; }
		
		virtual void set_clear_color(const color_t &value);
		
		virtual color_t get_clear_color() const;
		
		virtual void set_depth_value(GLfloat value);
		
		virtual void set_depth_function(GLenum value);
		
		virtual void clear_buffers(GLbitfield mask);
		
		virtual void enable_server_state(const GLenum cap);
		
		virtual void disable_server_state(const GLenum cap);
		
		virtual void enable_vertex_attr_array(GLuint index);
		
		virtual void disable_vertex_attr_array(GLuint index);
		
		virtual void set_blend_func(const GLenum sfactor, const GLenum dfactor);
		
		virtual void draw_elements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
		
		virtual GLuint create_shader(GLenum type, const std::string& source);
		
		virtual GLuint create_program(GLuint vertex_shader, GLuint fragment_shader);
		
		virtual void use_program(GLuint program_id);
		
		virtual void detach_shader(GLuint program_id, GLuint shader_id);
		
		virtual void delete_shader(GLuint shader_id);
		
		virtual void delete_program(GLuint program_id);
		
		virtual GLint get_uniform_location(GLuint program, const GLchar *name);
		
		virtual GLint get_attrib_location(GLuint program, const GLchar *name);
		
		virtual void set_uniform_matrix(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value, const unsigned char &dimension);
		
		virtual void set_uniform_vector(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
		
		virtual void set_uniform_value(GLint location, GLint v0);
		
		virtual void set_uniform_value(GLint location, GLfloat v0);
		
		virtual void set_pointer_vertex_attr(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
		
		virtual void gen_textures(GLsizei n, GLuint *textures);
		
		virtual void del_textures(GLsizei n, const GLuint * textures);
		
		virtual void bind_texture(GLuint texture, GLenum unit);
		
		virtual bool is_texture_binded(GLuint texture, GLenum unit);
		
		virtual void select_texture(GLenum unit);
		
		virtual void set_tex_param(GLenum target, GLenum pname, GLint param);
		
		virtual void load_texture(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border,
			GLenum format, GLenum type, const GLvoid *data);
		
		virtual ~Context();
		
	protected:
		virtual void set_viewport(const rectangle_t &value);
		
		Context();
		
	private:
		struct internal;
		std::unique_ptr<internal> m;						///< Внутренние данные контекста
		
		/** \brief Проверить статус шейдерной программы
		 * \param program_id Идентификатор шейдерной программы.
		 * \param param Параметр, который следует проверить (GL_LINK_STATUS, GL_VALIDATE_STATUS, GL_ATTACHED_SHADERS...)
		 * \return Возвращает значение проверяемого параметра
		 */
		GLint check_program_status(GLuint program_id, GLenum param);
		
		/** \brief Получить описание типа шейдера
		 */
		static std::string shader_type(GLenum type);
	};
	
	
	
	/** \brief не активный GL-контекст
	 */
	class ContextStub : public ContextBase
	{
	private:
		friend class gl;
		
	public:
		virtual inline bool is_active() { return false; }
		
		virtual void set_clear_color(const color_t &/*value*/) {}
		
		virtual color_t get_clear_color() const { return color_t(); }
		
		virtual void set_depth_value(GLfloat /*value*/) {}
		
		virtual void set_depth_function(GLenum /*value*/) {}
		
		virtual void clear_buffers(GLbitfield /*mask*/) {}
		
		virtual void enable_server_state(const GLenum /*cap*/) {}
		
		virtual void disable_server_state(const GLenum /*cap*/) {}
		
		virtual void enable_vertex_attr_array(GLuint /*index*/) {}
		
		virtual void disable_vertex_attr_array(GLuint /*index*/) {}
		
		virtual void set_blend_func(const GLenum /*sfactor*/, const GLenum /*dfactor*/) {}
		
		virtual void draw_elements(GLenum /*mode*/, GLsizei /*count*/, GLenum /*type*/, const GLvoid */*indices*/) {}
		
		/**
		 * Необходимо вернуть значение >0, чтобы в отсутствии контекста приложение не упало в assertion fail
		 */
		virtual GLuint create_shader(GLenum /*type*/, const std::string& /*source*/) { return 1; }
		
		/**
		 * Необходимо вернуть значение >0, чтобы в отсутствии контекста приложение не упало в assertion fail
		 */
		virtual GLuint create_program(GLuint /*vertex_shader*/, GLuint /*fragment_shader*/) { return 1; }
		
		virtual void use_program(GLuint /*program_id*/) {}
		
		virtual void detach_shader(GLuint /*program_id*/, GLuint /*shader_id*/) {}
		
		virtual void delete_shader(GLuint /*shader_id*/) {}
		
		virtual void delete_program(GLuint /*program_id*/) {}
		
		virtual GLint get_uniform_location(GLuint /*program*/, const GLchar */*name*/) { return 0; }
		
		virtual GLint get_attrib_location(GLuint /*program*/, const GLchar */*name*/) { return 0; }
		
		virtual void set_uniform_matrix(GLint /*location*/, GLsizei /*count*/, GLboolean /*transpose*/, const GLfloat */*value*/, const unsigned char &/*dimension*/) {}
		
		virtual void set_uniform_vector(GLint /*location*/, GLfloat /*v0*/, GLfloat /*v1*/, GLfloat /*v2*/, GLfloat /*v3*/) {}
		
		virtual void set_uniform_value(GLint /*location*/, GLint /*v0*/) {}
		
		virtual void set_uniform_value(GLint /*location*/, GLfloat /*v0*/) {}
		
		virtual void set_pointer_vertex_attr(GLuint /*index*/, GLint /*size*/, GLenum /*type*/, GLboolean /*normalized*/, GLsizei /*stride*/, const GLvoid */*pointer*/) {}
		
		virtual void gen_textures(GLsizei /*n*/, GLuint */*textures*/) {}
		
		virtual void del_textures(GLsizei /*n*/, const GLuint */*textures*/) {}
		
		virtual void bind_texture(GLuint /*texture*/, GLenum /*unit*/) {}
		
		virtual bool is_texture_binded(GLuint /*texture*/, GLenum /*unit*/) { return true; }
		
		virtual void select_texture(GLenum /*unit*/) {}
		
		virtual void set_tex_param(GLenum /*target*/, GLenum /*pname*/, GLint /*param*/) {}
		
		virtual void load_texture(GLenum /*target*/, GLint /*level*/, GLint /*internalFormat*/, GLsizei /*width*/, GLsizei /*height*/, GLint /*border*/,
			GLenum /*format*/, GLenum /*type*/, const GLvoid */*data*/) {}
		
		virtual ~ContextStub() {}
		
	protected:
		virtual void set_viewport(const rectangle_t &value) { }
		
		ContextStub() {}
	};
	
	
	
	inline void ctx_create() { gl::create(); }
	
	inline void ctx_destroy() { gl::destroy(); }
	
	inline ContextBase& ctx() { return *gl::ctx(); }
}

#endif // GRAPHICS_CONTEXT_HPP
