#ifndef GRAPHICS_TYPES_HPP
#define GRAPHICS_TYPES_HPP

#include <graphics/gl.hpp>


namespace graphics
{
	
	/** @brief Тип шейдерной программы
	 */
	enum shader_program_t
	{
		sp_simple = 0
	};
	
	
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
	
	
	struct color_t
	{
		GLfloat r;
		GLfloat g;
		GLfloat b;
		GLfloat a;
		color_t() : r(0), g(0), b(0), a(1) {}
		explicit color_t(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1) : r(r), g(g), b(b), a(a) {}
		~color_t() {}
		
		static inline color_t f_dw(GLuint hex)
		{ return f_ub((hex & 0x00ff0000) >> 16, (hex & 0x0000ff00) >> 8, hex & 0x000000ff, (hex & 0xff000000) >> 24); }
		
		static inline color_t f_ub(GLubyte r, GLubyte g, GLubyte b, GLubyte a = 255)
		{ return color_t((GLfloat)r / 255.0f, (GLfloat)g / 255.0f, (GLfloat)b / 255.0f, (GLfloat)a / 255.0f); }
	};
	
	inline bool operator ==(const color_t& lhs, const color_t& rhs)
	{ return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a; }
	
	inline bool operator !=(const color_t& lhs, const color_t& rhs)
	{ return !(lhs==rhs); }
	
}

#endif //GRAPHICS_TYPES_HPP
