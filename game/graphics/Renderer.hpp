#ifndef GRAPHICS_RENDERER_HPP
#define GRAPHICS_RENDERER_HPP


namespace graphics
{
	
	class Renderer
	{
	public:
		Renderer();
		
		~Renderer();
		
		inline bool is_valid() { return m_valid; }
		
		void on_surface_changed(int width, int height, bool is_tablet);
		
		void on_draw_frame();
		
	private:
		bool m_valid;
		float m_xr, m_yr, m_zr;
		
		void draw_cube();
	};
	
}

#endif //GRAPHICS_RENDERER_HPP
