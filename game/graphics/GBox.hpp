#ifndef GRAPHICS_GBOX_HPP
#define GRAPHICS_GBOX_HPP

#include <graphics/GObject.hpp>


namespace graphics
{
	
	class GBox : public GObject
	{
	private:
		typedef GObject super;
		
	public:
		GBox(const box_t &box);
		
		virtual ~GBox();
		
		/** \brief Установить границы коробки
		 */
		void set_box(const box_t &value);
		
		/** \brief Получить границы коробки
		 */
		inline const box_t& box() { return m_box; }
		
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
		box_t m_box;
	};
	
}

#endif // GRAPHICS_GBOX_HPP
