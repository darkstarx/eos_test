#include <freetype/types.hpp>


namespace utils
{
	
	std::string to_string(const freetype::glyph_outline_t &v)
	{
		switch (v)
		{
			case freetype::glyph_outline_t::none:
				return "none";
			case freetype::glyph_outline_t::in:
				return "in";
			case freetype::glyph_outline_t::out:
				return "out";
			case freetype::glyph_outline_t::full:
				return "full";
			default:
				return "unknown";
		}
	}
	
}
