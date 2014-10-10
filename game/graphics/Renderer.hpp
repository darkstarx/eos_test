#ifndef GRAPHICS_RENDERER_HPP
#define GRAPHICS_RENDERER_HPP

#include <graphics/types.hpp>
#include <graphics/forwards.hpp>
#include <map>


namespace graphics
{
	
	/** @brief Отрисовщик сцены
	 */
	class Renderer
	{
	private:
		typedef std::map<shader_program_t, ShaderProgramSPtr> shader_programs_t;
		
	public:
		Renderer(const Renderer& root) = delete;
		
		Renderer& operator=(const Renderer&) = delete;
		
		static void create();
		
		static void destroy();
		
		inline static bool alive() { return m_instance != 0; }
		
		inline static Renderer& instance() { return *m_instance; }
		
		inline bool is_valid() { return m_valid; }
		
		/** @brief Обработать смену GL-поверхности
		 * @param width Ширина GL-поверхности.
		 * @param height Высота GL-поверхности.
		 * @param is_tablet Признак того, что устройство является планшетом (иначе телефон).
		 */
		void on_surface_changed(int width, int height, bool is_tablet);
		
		/** @brief Обработать необходимость перерисовать кадр
		 */
		void on_draw_frame();
		
	private:
		static Renderer* m_instance;
		bool m_valid;							///< Признак актуальности текущего кадра
		shader_programs_t m_shader_programs;	///< Шейдерные программы
		float m_xr, m_yr, m_zr;
		
		Renderer();
		
		~Renderer();
		
		/** @brief Загрузить шейдеры и подготовить шейдерные программы
		 */
		void load_shaders();
		
		/** @brief Удалить все шейдеры и шейдерные программы
		 */
		void release_shaders();
		
		void draw_cube();
	};
	
}

inline void renderer_create() { graphics::Renderer::create(); }

inline void renderer_destroy() { graphics::Renderer::destroy(); }

inline bool renderer_alive() { return graphics::Renderer::alive(); }

inline graphics::Renderer& renderer() { return graphics::Renderer::instance(); }

#endif //GRAPHICS_RENDERER_HPP
