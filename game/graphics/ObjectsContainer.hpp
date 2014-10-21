#ifndef GRAPHICS_OBJECTSCONTAINER_H
#define GRAPHICS_OBJECTSCONTAINER_H

#include <graphics/forwards.hpp>
#include <utils/spin_lock.hpp>
#include <utils/enable_shared_from_this.hpp>
#include <list>


namespace graphics
{
	
	class ObjectsContainer
	{
	public:
		typedef std::list<GObjectSPtr> objects_t;
		
	public:
		ObjectsContainer();
		
		virtual ~ObjectsContainer();
		
		/** \brief Добавить объект в контейнер
		 */
		void add_object(const GObjectSPtr &object);
		
		/** \brief Вставить объект в контейнер на место первого объекта
		 * Остальные объекты сдвигаются вправо.
		 */
		void insert_first(const GObjectSPtr &object);
		
		/** \brief Вставить объект в контейнер на место перед указанным объектом
		 * Остальные объекты сдвигаются вправо.
		 */
		void insert_object(const GObjectSPtr &object, const GObjectSPtr &next_object);
		
		/** \brief Удалить объект из контейнера
		 */
		virtual void remove_object(const GObjectSPtr &object);
		
		/** \brief Удалить все объекты из контейнера
		 */
		virtual void clear();
		
		/** \brief Обработать сигнал об изменении графики
		 * Вызывается вложенными объектами, когда изменяется их графическое представление
		 */
		virtual void graphics_updated();
		
		/** \brief Получить список объектов контейнера
		 */
		inline const objects_t& objects() { return m_objects; }
		
	protected:
		utils::spin_lock m_list_lock;		///< Защита списка объектов
		
		/** \brief Выполнить вставку объекта в контейнер
		 */
		virtual void insert_object_impl(const GObjectSPtr &object, bool first, const GObjectSPtr &next_object);
		
	private:
		objects_t m_objects;
	};
	
}

#endif // GRAPHICS_OBJECTSCONTAINER_H
