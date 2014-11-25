#include <graphics/GContainer.hpp>
#include <utils/log.hpp>


namespace graphics
{
	
	GContainer::GContainer()
	: super()
	, ObjectsContainer()
	{
	}
	
	
	GContainer::~GContainer()
	{
	}
	
	
	void GContainer::graphics_updated()
	{
		ObjectsContainer::graphics_updated();
		// Если это первичный графический контейнер, то он обязан оповестить отрисовщика о том,
		// что графика изменилась, и её следует перерисовать, либо оповестить об этом владельца
		if (m_prime) {
			renderer().invalidate();
		} else invalidate();
	}
	
	
	void GContainer::do_draw()
	{
		utils::scoped_lock guard(m_list_lock);
		for (objects_t::const_iterator it = objects().begin(), end = objects().end(); it != end; ++it)
		{
			const GObjectSPtr &object = *it;
			object->draw();
		}
	}
	
}
