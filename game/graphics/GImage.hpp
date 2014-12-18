#ifndef GRAPHICS_GIMAGE_HPP
#define GRAPHICS_GIMAGE_HPP

#include <graphics/GMeshable.hpp>


namespace graphics
{
	
	class GImage : public GMeshable
	{
	private:
		typedef GMeshable super;
		
	public:
		GImage(const rectangle_t &rect);
		
		virtual ~GImage();
		
		/** \brief Установить позицию и размеры виджета
		 */
		void set_rectangle(const rectangle_t &value);
		
		/** \brief Получить позицию и размеры виджета
		 */
		inline const rectangle_t& rectangle() { return m_rect; }
		
		/** \brief Установить текстуру изображения
		 */
		void set_image(const graphics::TextureSPtr &texture);
		
		/** \brief Установить текстуру маски
		 */
		void set_mask(const graphics::TextureSPtr &texture);
		
	protected:
		std::shared_ptr<GLfloat> m_img_coords;	///< Текстурные координаты изображения
		std::shared_ptr<GLfloat> m_msk_coords;	///< Текстурные координаты маски
		GLsizei m_img_coords_count;				///< Кол-во элементов в буфере текстурных координат изображения
		GLsizei m_msk_coords_count;				///< Кол-во элементов в буфере текстурных координат маски
		
		/** \brief Пометить буфер текстурных координат изображения не актуальным
		 */
		inline void invalidate_img_coords() { m_img_coords_valid = false; }
		
		/** \brief Пометить буфер текстурных координат маски не актуальным
		 */
		inline void invalidate_msk_coords() { m_msk_coords_valid = false; }
		
		/** \brief Обновить буфер текстурных координат изображения
		 */
		virtual void update_img_coords();
		
		/** \brief Обновить буфер текстурных координат маски
		 */
		virtual void update_msk_coords();
		
	protected:
		/** \brief Обновить буфер вершин
		 */
		virtual void update_vertices();
		
		/** \brief Обновить буфер индексов
		 */
		virtual void update_indices();
		
		/** \brief Выполнитть действия перед отрисовкой
		 */
		virtual void before_draw();
		
		/** \brief Выбрать и подготовить шейдерную программу
		 */
		virtual void set_program();
		
		/** \brief Выполнить отрисовку объекта
		 */
		virtual void do_draw();
		
	private:
		rectangle_t m_rect;
		TextureSPtr m_tex_img;					///< Текстура изображения
		TextureSPtr m_tex_msk;					///< Текстура маски
		bool m_img_coords_valid;				///< Признак актуальности буфера текстурных координат изображения
		bool m_msk_coords_valid;				///< Признак актуальности буфера текстурных координат маски
	};
	
}

#endif // GRAPHICS_GIMAGE_HPP
