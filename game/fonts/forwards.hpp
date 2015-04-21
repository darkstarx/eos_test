#ifndef FONTS_FORWARDS_HPP
#define FONTS_FORWARDS_HPP

#include <memory>


namespace fonts
{
	
	class Font;
	typedef std::shared_ptr<Font> FontSPtr;
	typedef std::weak_ptr<Font> FontWPtr;
	
}

#endif // FONTS_FORWARDS_HPP
