#ifndef GRAPHICS_FORWARDS_HPP
#define GRAPHICS_FORWARDS_HPP

#include <memory>


namespace graphics
{
	
	class ShaderProgram;
	typedef std::shared_ptr<ShaderProgram> ShaderProgramSPtr;
	typedef std::weak_ptr<ShaderProgram> ShaderProgramWPtr;
	
	class GObject;
	typedef std::shared_ptr<GObject> GObjectSPtr;
	typedef std::weak_ptr<GObject> GObjectWPtr;
	
	class GMeshable;
	typedef std::shared_ptr<GMeshable> GMeshableSPtr;
	typedef std::weak_ptr<GMeshable> GMeshableWPtr;
	
	class ObjectsContainer;
	typedef ObjectsContainer* ObjectsContainerPtr;
	typedef std::shared_ptr<ObjectsContainer> ObjectsContainerSPtr;
	
	class GContainer;
	typedef std::shared_ptr<GContainer> GContainerSPtr;
	typedef std::weak_ptr<GContainer> GContainerWPtr;
	
	class GRectangle;
	typedef std::shared_ptr<GRectangle> GRectangleSPtr;
	typedef std::weak_ptr<GRectangle> GRectangleWPtr;
	
	class GImage;
	typedef std::shared_ptr<GImage> GImageSPtr;
	typedef std::weak_ptr<GImage> GImageWPtr;
	
	class GImages;
	typedef std::shared_ptr<GImages> GImagesSPtr;
	typedef std::weak_ptr<GImages> GImagesWPtr;
	
	class Texture;
	typedef Texture* TexturePtr;
	typedef std::shared_ptr<Texture> TextureSPtr;
	typedef std::weak_ptr<Texture> TextureWPtr;
	
}

#endif // GRAPHICS_FORWARDS_HPP
