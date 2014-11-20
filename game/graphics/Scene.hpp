#ifndef GRAPHICS_SCENE_HPP
#define GRAPHICS_SCENE_HPP

#include <graphics/forwards.hpp>
#include <list>


namespace graphics
{
	
	class Scene
	{
	private:
		typedef std::list<GObjectSPtr> objects_t;
		
	public:
		Scene();
		
		~Scene();
		
		void draw();
		
	private:
		objects_t m_objects;
	};
	
}

#endif // GRAPHICS_SCENE_HPP
