#ifndef GRAPHICS_RENDERER_HPP
#define GRAPHICS_RENDERER_HPP

#include <graphics/types.hpp>
#include <graphics/forwards.hpp>
#include <utils/spin_lock.hpp>
#include <utils/log.hpp>
#include <Application.hpp>
#include <map>


namespace graphics
{
	
	class Renderer;
	
	
	class RendererDestroyer
	{
	public:
		~RendererDestroyer();
		inline void init(Renderer *instance) { ASSERT(!m_instance); m_instance = instance; }
	private:
		Renderer *m_instance;
	};
	
	
	/** \brief Отрисовщик сцены
	 */
	class Renderer
	{
	private:
		friend class RendererDestroyer;
		friend class gl;
		
		typedef std::map<shader_program_t, ShaderProgramSPtr> shader_programs_t;
		
	public:
		Renderer(const Renderer& root) = delete;
		
		Renderer& operator=(const Renderer&) = delete;
		
		static bool is_alive() { return m_instance; }
		
		static Renderer& instance();
		
		void on_ctx_create();
		
		void on_ctx_destroy();
		
		void set_graphics(const GContainerSPtr &container);
		
		inline GContainerSPtr graphics() { return m_graphics; }
		
		void invalidate();
		
		inline bool is_valid() { return m_valid; }
		
		/** \brief Обработать смену GL-поверхности
		 * NOTE Выполняется в графическом потоке.
		 * \param width Ширина GL-поверхности.
		 * \param height Высота GL-поверхности.
		 * \param is_tablet Признак того, что устройство является планшетом (иначе телефон).
		 */
		void on_surface_changed(int width, int height, bool is_tablet);
		
		/** \brief Обработать необходимость перерисовать кадр
		 * NOTE Выполняется в графическом потоке.
		 */
		void on_draw_frame();
		
		/** \brief Получить шейдерную программу
		 */
		ShaderProgramSPtr get_program(shader_program_t program_type);
		
	private:
		static Renderer* m_instance;			///< Единственный экземпляр отрисовщика
		static RendererDestroyer m_destroyer;	///< Разрушитель экземпляра рендерера
		bool m_valid;							///< Признак того, что графику следует перерисовать
		shader_programs_t m_shader_programs;	///< Шейдерные программы
		GContainerSPtr m_graphics;				///< Первичный контейнер графических объектов
		utils::spin_lock m_graphics_lock;		///< Защита первичного графического объекта и флага актуальности графики
		
		Renderer();
		
		~Renderer();
		
		/** \brief Загрузить шейдеры и подготовить шейдерные программы
		 * NOTE Выполняется в графическом потоке.
		 */
		void load_shaders();
		
		/** \brief Выполнить загрузку вершинного и фрагментного шейдеров и подготовить шейдерную программу
		 * NOTE Выполняется в графическом потоке.
		 */
		void load_program(const std::string &name, shader_program_t program_type);
		
		/** \brief Удалить все шейдеры и шейдерные программы
		 * NOTE Выполняется в графическом потоке.
		 */
		void release_shaders();
	};
	
}

inline graphics::Renderer& renderer() { return graphics::Renderer::instance(); }

#endif //GRAPHICS_RENDERER_HPP
