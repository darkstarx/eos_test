#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <graphics/types.hpp>
#include <graphics/gl.hpp>
#include <glm/glm.hpp>
#include <string>


namespace graphics
{
	
	/** \brief Шейдерная программа
	 */
	class ShaderProgram
	{
	public:
		enum
		{
			INVALID_ID = -1
		};
		
	private:
		struct uniforms_t
		{
			GLint u_mvpmatrix;
			GLint u_color, u_texture, u_mask, u_texmatrix;
			uniforms_t()
			: u_mvpmatrix(INVALID_ID)
			, u_color(INVALID_ID)
			, u_texture(INVALID_ID)
			, u_mask(INVALID_ID)
			, u_texmatrix(INVALID_ID)
			{}
		} uniforms;
		
		struct uniform_values_t
		{
			glm::mat4 u_mvpmatrix;
			glm::mat3 u_texmatrix;
			color_t u_color;
			uniform_values_t()
			: u_mvpmatrix(0.0f)
			, u_texmatrix(0.0f)
			, u_color(0.0f, 0.0f, 0.0f, 0.0f)
			{}
		} uniform_values;
		
		struct attributes_t
		{
			GLint a_position;
			GLint a_texcoord, a_masktexcoord, a_color;
			attributes_t()
			: a_position(INVALID_ID)
			, a_texcoord(INVALID_ID)
			, a_masktexcoord(INVALID_ID)
			, a_color(INVALID_ID)
			{}
		} attributes;
		
	public:
		ShaderProgram(const std::string &vertex_src, const std::string &fragment_src);
		
		~ShaderProgram();
		
		/** \brief Инициализировать шейдерную программу
		 */
		void initialize(const std::string &vertex_src, const std::string &fragment_src);
		
		/** \brief Деинициализировать шейдерную программу
		 */
		void deinitialize();
		
		/** \brief Использовать данную шейдерную программу
		 */
		void use();
		
		/** \brief Установить любое вещественное значение в указанную униформу
		 * Если шейдерная программа поддерживает указанную юниформу name, устанавливает
		 * ей указанное значение value.
		 */
		void set_uniform(const GLfloat value, const std::string& name);
		
		/** \brief Установить MVP-матрицу трансформаций
		 */
		void set_uniform_mvpmatrix(const glm::mat4 &matrix);
		
		/** \brief Проверить наличие униформы цвета у шейдерной программы
		 */
		inline bool has_uniform_color() const { return uniforms.u_color != INVALID_ID; }
		
		/** \brief Установить значение для униформы цвета
		 */
		void set_uniform_color(const color_t &color);
		
		/** \brief Установить значение для униформы текстуры
		 */
		void set_uniform_texture(GLint index);
		
		/** \brief Установить значение для униформы маски
		 */
		void set_uniform_mask(GLint index);
		
		/** \brief Установить значение для униформы матрицы трансформаций текстурных координат
		 */
		void set_uniform_texmatrix(const glm::mat3 &matrix);
		
		/** \brief Установить значение для атрибута позиций
		 */
		void set_attribute_position2(const void *data, GLsizei stride = 0);
		
		/** \brief Установить значение для атрибута позиций
		 */
		void set_attribute_position3(const void *data, GLsizei stride = 0);
		
		/** \brief Установить значение для атрибута текстурных координат изображения
		 */
		void set_attribute_texcoord(const void *data, GLsizei stride = 0);
		
		/** \brief Установить значение для атрибута текстурных координат маски
		 */
		void set_attribute_masktexcoord(const void *data, GLsizei stride = 0);
		
		/** \brief Установить значение для атрибута цветового буфера
		 */
		void set_attribute_color(const void *data, GLsizei stride = 0);
		
		/** \brief Сбросить все установки буферов для атрибутов
		 */
		void reset_attributes();
		
	private:
		GLuint m_vertex_shader;		///< Идентификатор вершинного шейдера
		GLuint m_fragment_shader;	///< Идентификатор фрагментного шейдера
		GLuint m_program;			///< Идентификатор шейдерной программы
	};
	
}

#endif // SHADERPROGRAM_HPP
