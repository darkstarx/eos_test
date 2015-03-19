#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

#include <graphics/gl.hpp>
#include <graphics/types.hpp>
#include <resources/forwards.hpp>
#include <utils/enable_shared_from_this.hpp>
#include <string>


namespace graphics
{
	
	class Texture : public utils::enable_shared_from_this
	{
	public:
		Texture(GLenum unit = GL_TEXTURE0);
		
		~Texture();
		
		/** \brief Инициализировать объект текстуры
		 * Генерирует объект текстуры и загружает изображение в видеопамять.
		 */
		void initialize(const resources::ImageSPtr &image);
		
		/** \brief Деинициализировать объект текстуры
		 * Освобождает видеопамять, удаляет объект текстуры.
		 */
		void deinitialize();
		
		/** \brief Получить признак инициализированности текстуры
		 */
		inline bool initialized() { return m_initialized; }
		
		/** \brief Использовать данную текстуру для отрисовки
		 */
		void use();
		
		/** \brief Получить идентификатор текстуры
		 */
		inline GLuint id() { return m_id; }

		/** \brief Получить размеры тестуры
		*/
		inline dimension_t& dimension() { return m_dimension; }
		
		/** \brief Установить текстурный юнит, к которому привязывается текстура
		 */
		inline void set_unit(GLenum value) { m_unit = value; }
		
		/** \brief Получить текстурный юнит, к которому привязывается текстура
		 */
		inline GLenum unit() { return m_unit; }
		
		/** \brief Получить индекс текстурного юнита, к которому привязывается текстура
		 */
		inline GLint unit_index() { return m_unit - GL_TEXTURE0; }
		
	private:
		bool m_initialized;	      ///< Признак того, что текстура инициализирована
		GLuint m_id;		      ///< Идентификатор текстуры
		GLenum m_unit;		      ///< Текстурный юнит, для которого предназначена данная текстура
		dimension_t m_dimension;  ///< Размеры текстуры
	};
	
}

#endif // GRAPHICS_TEXTURE_H
