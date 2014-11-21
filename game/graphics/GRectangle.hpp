#ifndef GRAPHICS_GRECTANGLE_HPP
#define GRAPHICS_GRECTANGLE_HPP

#include <graphics/GMeshable.hpp>


namespace graphics
{
	
	class GRectangle : public GMeshable
	{
	private:
		typedef GMeshable super;
		
	public:
		GRectangle(const rectangle_t &rect);
		
		virtual ~GRectangle();
		
		/** \brief Установить позицию и размеры виджета
		 */
		void set_rectangle(const rectangle_t &value);
		
		/** \brief Получить позицию и размеры виджета
		 */
		inline const rectangle_t& rectangle() { return m_rect; }
		
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
		
		/** \brief Выполнить отрисовку объекта
		 */
		virtual void do_draw();
		
	private:
		rectangle_t m_rect;
	};
	
}

#endif // GRAPHICS_GRECTANGLE_HPP
