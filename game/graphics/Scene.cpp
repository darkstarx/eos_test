#include <graphics/Scene.hpp>
#include <graphics/GObject.hpp>


namespace graphics
{
	
	Scene::Scene()
	{
	}
	
	
	Scene::~Scene()
	{
	}
	
	
	void Scene::draw()
	{
		for (objects_t::iterator it = m_objects.begin(), end = m_objects.end(); it != end; ++it)
		{
			GObjectSPtr &object = *it;
			object->draw();
		}
	}
	
}