#ifndef GRAPHICS_GCONTAINER_H
#define GRAPHICS_GCONTAINER_H

#include <graphics/GObject.hpp>
#include <graphics/ObjectsContainer.hpp>


namespace graphics
{
	
	class GContainer : public GObject, public ObjectsContainer
	{
	private:
		typedef GObject super;
		
		friend void Renderer::set_graphics(const GContainerSPtr& container);
		
	public:
		GContainer();
		
		virtual ~GContainer();
		
		/** \brief Обработать сигнал об изменении графики
		 * Вызывается вложенными объектами, когда изменяется их графическое представление
		 */
		virtual void graphics_updated();
		
	protected:
		/** \brief Выполнить отрисовку объекта
		 */
		virtual void do_draw();
		
	private:
		bool m_prime;		///< Признак того, что контейнер является первичным графическим объектом
		
		inline void set_prime(bool value) { m_prime = value; }
	};
	
}

#endif // GRAPHICS_GCONTAINER_H
