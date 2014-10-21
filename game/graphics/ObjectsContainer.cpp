#include <graphics/ObjectsContainer.hpp>
#include <graphics/GObject.hpp>
#include <utils/log.hpp>
#include <algorithm>


namespace graphics
{
	
	ObjectsContainer::ObjectsContainer()
	{
	}
	
	
	ObjectsContainer::~ObjectsContainer()
	{
		clear();
	}
	
	
	void ObjectsContainer::add_object(const GObjectSPtr& object)
	{
		insert_object_impl(object, false, GObjectSPtr());
	}
	
	
	void ObjectsContainer::insert_first(const GObjectSPtr& object)
	{
		insert_object_impl(object, true, GObjectSPtr());
	}
	
	
	void ObjectsContainer::insert_object(const GObjectSPtr& object, const GObjectSPtr& next_object)
	{
		insert_object_impl(object, false, next_object);
	}
	
	
	void ObjectsContainer::remove_object(const GObjectSPtr& object)
	{
		ASSERT(object) << "Невозможно удалить из контейнера пустой объект";
		ASSERT(object->m_owner) << "Невозможно удалить объект из контейнера, который не является владельцем этого объекта";
		utils::scoped_lock guard(m_list_lock);
		m_objects.remove(object);
		object->m_owner = 0;
		graphics_updated();
	}
	
	
	void ObjectsContainer::clear()
	{
		utils::scoped_lock guard(m_list_lock);
		for (objects_t::const_iterator it = m_objects.begin(), end = m_objects.end(); it != end; ++it)
		{
			const GObjectSPtr &object = *it;
			ASSERT(object->m_owner) << "Невозможно удалить объект из контейнера, который не является владельцем этого объекта";
			object->m_owner = 0;
		}
		m_objects.clear();
		graphics_updated();
	}
	
	
	void ObjectsContainer::insert_object_impl(const GObjectSPtr& object, bool first, const GObjectSPtr& next_object)
	{
		ASSERT(object) << "Не разрешается добавлять в контейнер пустые объекты";
		ASSERT(!object->m_owner) << "Чтобы добавить объект в другой контейнер, его следует удалить из текущего";
		utils::scoped_lock guard(m_list_lock);
		if (first) {
			m_objects.insert(m_objects.begin(), object);
		} else if (next_object) {
			objects_t::iterator pos = std::find(m_objects.begin(), m_objects.end(), next_object);
			if (pos == m_objects.end()) return;
			m_objects.insert(pos, object);
		} else {
			m_objects.push_back(object);
		}
		object->m_owner = this;
		graphics_updated();
	}
	
	
	void ObjectsContainer::graphics_updated()
	{
	}
	
}
