#ifndef GRAPHICS_TYPES_HPP
#define GRAPHICS_TYPES_HPP

#include <graphics/gl.hpp>


namespace graphics
{
	
	struct position_t
	{
		GLfloat x;
		GLfloat y;
		position_t() : x(0), y(0) {}
		explicit position_t(GLfloat x, GLfloat y) : x(x), y(y) {}
		~position_t() {}
		inline bool operator !() const { return x == 0 && y == 0; }
	};
	
	struct dimension_t
	{
		GLfloat w;
		GLfloat h;
		dimension_t() : w(0), h(0) {}
		explicit dimension_t(GLfloat w, GLfloat h) : w(w), h(h) {}
		~dimension_t() {}
		inline bool operator !() const { return w == 0 && h == 0; }
		
	};
	
	struct rectangle_t : public position_t, public dimension_t
	{
		rectangle_t() : position_t(), dimension_t() {}
		explicit rectangle_t(GLfloat x, GLfloat y, GLfloat w, GLfloat h) : position_t(x, y), dimension_t(w, h) {}
		explicit rectangle_t(const position_t& pos, GLfloat w, GLfloat h) : position_t(pos), dimension_t(w, h) {}
		explicit rectangle_t(GLfloat x, GLfloat y, const dimension_t& dim) : position_t(x, y), dimension_t(dim) {}
		explicit rectangle_t(const position_t& pos, const dimension_t& dim) : position_t(pos), dimension_t(dim) {}
		explicit rectangle_t(GLfloat x, GLfloat y, const rectangle_t& rect) : position_t(x,y), dimension_t(rect.w, rect.h) {}
		~rectangle_t() {}
		inline position_t& position() { return *this; }
		inline const position_t& position() const { return *this; }
		inline dimension_t& dimension() { return *this; }
		inline const dimension_t& dimension() const { return *this; }
		inline bool operator !() const { return x == 0 && y == 0 && w == 0 && h == 0; }
	};
	
}

#endif //GRAPHICS_TYPES_HPP
