#ifndef GRAPHICS_FORWARDS_HPP
#define GRAPHICS_FORWARDS_HPP

#include <memory>


namespace graphics
{
	
	class ShaderProgram;
	typedef std::shared_ptr<ShaderProgram> ShaderProgramSPtr;
	
	class GObject;
	typedef std::shared_ptr<GObject> GObjectSPtr;
	
	class GBox;
	typedef std::shared_ptr<GBox> GBoxSPtr;
	
}

#endif // GRAPHICS_FORWARDS_HPP
