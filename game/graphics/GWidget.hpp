#ifndef GWIDGET_HPP
#define GWIDGET_HPP

// #include <graphics/types.hpp>
// #include <memory>
// 
// 
// namespace graphics
// {
// 	
// 	class GWidget
// 	{
// 	public:
// 		GWidget();
// 		
// 		virtual ~GWidget();
// 		
// 		/** \brief Установить границы виджета
// 		 */
// 		void set_rectangle(const rectangle_t &rectangle);
// 		
// 		/** \brief Получить границы виджета
// 		 */
// 		inline const rectangle_t& rectangle() const { return m_rectangle; }
// 		
// 		/** \brief Установить центр трансформации
// 		 */
// 		void set_transform_point(const position_t &position);
// 		
// 		/** \brief Получить центр трансформации
// 		 */
// 		inline const position_t& transform_point() const { return m_transform_point; }
// 		
// 		/** \brief Установить вращение виджета
// 		 */
// 		void set_rotation(const rotation_t &rotation);
// 		
// 		/** \brief Получить вращение виджета
// 		 */
// 		inline const rotation_t& rotation() const { return m_rotation; }
// 		
// 		/** \brief Установить масштаб виджета
// 		 */
// 		void set_scale(const scale_t &scale);
// 		
// 		/** \brief Получить масштаб виджета
// 		 */
// 		inline const scale_t& scale() const { return m_scale; }
// 		
// 		/** \brief Установить цвет виджета
// 		 */
// 		void set_color(const color_t &color);
// 		
// 		/** \brief Получить цвет виджета
// 		 */
// 		inline const color_t& color() const { return m_color; }
// 		
// 		/** \brief Выполнить отрисовку виджета
// 		 */
// 		virtual void draw();
// 		
// 	protected:
// 		/** \brief Обновить буфер вершин
// 		 */
// 		virtual void update_vertices();
// 		
// 		/** \brief Обновить буфер текстурных координат изображения
// 		 */
// 		virtual void update_img_texcoords();
// 		
// 		/** \brief Обновить буфер текстурных координат маски
// 		 */
// 		virtual void update_msk_texcoords();
// 		
// 		/** \brief Обновить буфер индексов
// 		 */
// 		virtual void update_indices();
// 		
// 	private:
// 		rectangle_t m_rectangle;					///< Границы виджета
// 		position_t m_transform_point;				///< Центр трансформации виджета
// 		rotation_t m_rotation;						///< Вращение виджета
// 		scale_t m_scale;							///< Масштаб виджета
// 		color_t m_color;							///< Цвет виджета
// 		
// 		std::shared_ptr<GLfloat> m_vertices;		///< Буфер вершин
// 		std::shared_ptr<GLfloat> m_img_texcoords;	///< Буфер текстурных координат изображения
// 		std::shared_ptr<GLfloat> m_msk_texcoords;	///< Буфер текстурных координат маски
// 		std::shared_ptr<GLushort> m_indices;		///< Буфер индексов
// 	};
// 	
// }

#endif // GWIDGET_HPP
