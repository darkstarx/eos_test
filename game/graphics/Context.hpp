#ifndef GRAPHICS_CONTEXT_HPP
#define GRAPHICS_CONTEXT_HPP

#include <graphics/gl.hpp>
#include <vector>


namespace graphics
{
	
	class Context
	{
	public:
		Context(const Context& root) = delete;
		
		Context& operator=(const Context&) = delete;
		
		static void create();
		
		static void destroy();
		
		inline static bool alive() { return m_instance != 0; }
		
		inline static Context& instance() { return *m_instance; }
		
		void enable_vertex_attrib_array(GLuint index);
		
		void disable_vertex_attrib_array(GLuint index);
		
	private:
		static Context *m_instance;
		std::vector<bool> m_vertex_attrib_arrays_states;
		
		Context();
		
		~Context();
	};
	
	inline void ctx_create() { Context::create(); }
	
	inline void ctx_destroy() { Context::destroy(); }
	
	inline bool ctx_alive() { return Context::alive(); }
	
	inline Context& ctx() { return Context::instance(); }
}

#endif // GRAPHICS_CONTEXT_HPP
