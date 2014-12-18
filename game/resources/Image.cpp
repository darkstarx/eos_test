#include <resources/Image.hpp>


namespace resources
{
	
	Image::Image(const utils::bytearray &data, unsigned int width, unsigned int height, unsigned char components)
	: m_data(data)
	, m_width(width)
	, m_height(height)
	, m_components(components)
	{
	}
	
	
	Image::~Image()
	{
	}
	
}