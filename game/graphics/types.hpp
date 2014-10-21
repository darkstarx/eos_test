#ifndef GRAPHICS_TYPES_HPP
#define GRAPHICS_TYPES_HPP

#include <graphics/gl.hpp>


namespace graphics
{
	
	/** \brief Тип шейдерной программы
	 */
	enum shader_program_t
	{
		sp_simple = 0
	};
	
	
	struct position_t
	{
		GLfloat x;
		GLfloat y;
		GLfloat z;
		position_t() : x(0.0f), y(0.0f), z(0.0f) {}
		explicit position_t(GLfloat x, GLfloat y) : x(x), y(y), z(0.0f) {}
		explicit position_t(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {}
		~position_t() {}
		inline bool operator !() const { return x == 0.0f && y == 0.0f && z == 0.0f; }
	};
	
	inline bool operator ==(const position_t &lhs, const position_t &rhs)
	{ return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
	
	inline bool operator !=(const position_t &lhs, const position_t &rhs)
	{ return !(lhs == rhs); }
	
	
	struct dimension_t
	{
		GLfloat w;
		GLfloat h;
		GLfloat d;
		dimension_t() : w(0.0f), h(0.0f), d(0.0f) {}
		explicit dimension_t(GLfloat w, GLfloat h) : w(w), h(h), d(0.0f) {}
		explicit dimension_t(GLfloat w, GLfloat h, GLfloat d) : w(w), h(h), d(d) {}
		~dimension_t() {}
		inline bool operator !() const { return w == 0.0f && h == 0.0f && d == 0.0f; }
		
	};
	
	inline bool operator ==(const dimension_t &lhs, const dimension_t &rhs)
	{ return lhs.w == rhs.w && lhs.h == rhs.h && lhs.d == rhs.d; }
	
	inline bool operator !=(const dimension_t &lhs, const dimension_t &rhs)
	{ return !(lhs == rhs); }
	
	
	struct rectangle_t : public position_t, public dimension_t
	{
		rectangle_t() : position_t(), dimension_t() {}
		explicit rectangle_t(GLfloat x, GLfloat y, GLfloat w, GLfloat h) : position_t(x, y), dimension_t(w, h) {}
		explicit rectangle_t(const position_t& pos, GLfloat w, GLfloat h) : position_t(pos), dimension_t(w, h) {}
		explicit rectangle_t(GLfloat x, GLfloat y, const dimension_t& dim) : position_t(x, y), dimension_t(dim) {}
		explicit rectangle_t(const position_t& pos, const dimension_t& dim) : position_t(pos), dimension_t(dim) {}
		~rectangle_t() {}
		inline position_t& position() { return *this; }
		inline const position_t& position() const { return *this; }
		inline dimension_t& dimension() { return *this; }
		inline const dimension_t& dimension() const { return *this; }
		inline bool operator !() const { return x == 0.0f && y == 0.0f && w == 0.0f && h == 0.0f; }
	};
	
	inline bool operator ==(const rectangle_t &lhs, const rectangle_t &rhs)
	{ return lhs.x == rhs.x && lhs.y == rhs.y && lhs.w == rhs.w && lhs.h == rhs.h; }
	
	inline bool operator !=(const rectangle_t &lhs, const rectangle_t &rhs)
	{ return !(lhs == rhs); }
	
	
	struct box_t : public position_t, public dimension_t
	{
		box_t() : position_t(), dimension_t() {}
		explicit box_t(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h, GLfloat d) : position_t(x, y, z), dimension_t(w, h, d) {}
		explicit box_t(const position_t& pos, GLfloat w, GLfloat h, GLfloat d) : position_t(pos), dimension_t(w, h, d) {}
		explicit box_t(GLfloat x, GLfloat y, GLfloat z, const dimension_t& dim) : position_t(x, y, z), dimension_t(dim) {}
		explicit box_t(const position_t& pos, const dimension_t& dim) : position_t(pos), dimension_t(dim) {}
		~box_t() {}
		inline position_t& position() { return *this; }
		inline const position_t& position() const { return *this; }
		inline dimension_t& dimension() { return *this; }
		inline const dimension_t& dimension() const { return *this; }
		inline bool operator !() const { return x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f && h == 0.0f && d == 0.0f; }
	};
	
	inline bool operator ==(const box_t &lhs, const box_t &rhs)
	{ return lhs.position() == rhs.position() && lhs.dimension() == rhs.dimension(); }
	
	inline bool operator !=(const box_t &lhs, const box_t &rhs)
	{ return !(lhs == rhs); }
	
	
	struct rotation_t
	{
		GLfloat angle_x;
		GLfloat angle_y;
		GLfloat angle_z;
		
		rotation_t() : angle_x(0.0f), angle_y(0.0f), angle_z(0.0f) {}
		explicit rotation_t(GLfloat angle_x, GLfloat angle_y, GLfloat angle_z) : angle_x(angle_x), angle_y(angle_y), angle_z(angle_z) {}
		~rotation_t() {}
	};
	
	inline bool operator ==(const rotation_t &lhs, const rotation_t &rhs)
	{ return lhs.angle_x == rhs.angle_x && lhs.angle_y == rhs.angle_y && lhs.angle_z == rhs.angle_z; }
	
	inline bool operator !=(const rotation_t &lhs, const rotation_t &rhs)
	{ return !(lhs == rhs); }
	
	
	struct scale_t
	{
		GLfloat x;
		GLfloat y;
		GLfloat z;
		
		scale_t() : x(1.0f), y(1.0f), z(1.0f) {}
		explicit scale_t(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {}
		~scale_t() {}
	};
	
	inline bool operator ==(const scale_t &lhs, const scale_t &rhs)
	{ return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
	
	inline bool operator !=(const scale_t &lhs, const scale_t &rhs)
	{ return !(lhs == rhs); }
	
	
	struct color_t
	{
		GLfloat r;
		GLfloat g;
		GLfloat b;
		GLfloat a;
		color_t() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
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
