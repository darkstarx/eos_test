#ifndef GRAPHICS_FORWARDS_HPP
#define GRAPHICS_FORWARDS_HPP

#include <memory>


namespace graphics
{
	
	class ShaderProgram;
	typedef std::shared_ptr<ShaderProgram> ShaderProgramSPtr;
	
	class GObject;
	typedef std::shared_ptr<GObject> GObjectSPtr;
	
	class GMeshable;
	typedef std::shared_ptr<GMeshable> GMeshableSPtr;
	
	class ObjectsContainer;
	typedef ObjectsContainer* ObjectsContainerPtr;
	typedef std::shared_ptr<ObjectsContainer> ObjectsContainerSPtr;
	
	class GContainer;
	typedef std::shared_ptr<GContainer> GContainerSPtr;
	
	class GRectangle;
	typedef std::shared_ptr<GRectangle> GRectangleSPtr;
	
}

#endif // GRAPHICS_FORWARDS_HPP
