#ifndef RESOURCES_IMAGE_HPP
#define RESOURCES_IMAGE_HPP

#include <utils/bytearray.hpp>


namespace resources
{
	
	class Image {
	public:
		Image(const utils::bytearray &data, unsigned int width, unsigned int height, unsigned char components);
		
		~Image();
		
		/** \brief Получить изображение
		 */
		inline utils::bytearray& data() { return m_data; }
		
		/** \brief Получить изображение
		 */
		inline const utils::bytearray& data() const { return m_data; }
		
		/** \brief Получить ширину изображения в пикселах
		 */
		inline unsigned int width() const { return m_width; }
		
		/** \brief Получать высоту изображения в пикселах
		 */
		inline unsigned int height() const { return m_height; }
		
		/** \brief Получить кол-во компонентов на пиксел
		 * Один компонент занимает один байт. Например, в изображении формата RGB один пиксел содержит
		 * три компонента по одному байту на каждый. А в изображении формата RGBA - четыре компонента.
		 */
		inline unsigned char components() const { return m_components; }
		
	private:
		utils::bytearray m_data;	///< Изображение
		unsigned int m_width;		///< Ширина изображения в пикселах
		unsigned int m_height;		///< Высота изображения в пикселах
		unsigned char m_components;	///< Кол-во компонентов на пиксел
	};
	
}

#endif // RESOURCES_IMAGE_HPP
