#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <graphics/types.hpp>
#include <graphics/forwards.hpp>
#include <graphics/events.hpp>
#include <utils/forwards.hpp>


class Application
{
public:
	Application(const Application& root) = delete;
	
	Application& operator=(const Application&) = delete;
	
	static void create();
	
	static void destroy();
	
	inline static bool alive() { return m_instance != 0; }
	
	inline static Application& instance() { return *m_instance; }
	
	/** \brief Таймер приложения
	 * Метод вызывается раз в 60 секунд в основном потоке приложения, позволяет выполнять
	 * очень кратковременные периодические операции.
	 * WARNING Нельзя задерживать выполнение этого метода, так как он выполняется в том же потоке,
	 * что и обработчики событий ввода пользовательских команд!
	 */
	void tick();
	
	/** \brief Запуск приложения
	 * Метод выполняется при запуске приложения.
	 */
	void start();
	
	/** \brief Останов приложения
	 * Метод выполняется при остановке приложения.
	 */
	void stop();
	
	/** \brief Приостановка приложения
	 * Метод выполняется при временной приостановке приложения.
	 */
	void pause();
	
	/** \brief Возобновление работы приложения
	 * Метод выполняется при возобновлении работы приложения.
	 */
	void resume();
	
	/** \brief Обработать создание графики
	 * NOTE Метод выполняется в графическом потоке.
	 */
	void on_graphics_created();
	
	/** \brief Обработать разрушение графики
	 * NOTE Метод выполняется в графическом потоке.
	 */
	void on_graphics_destroyed();
	
	/** \brief Сообщение от манипулятора (тачпада или мыши)
	 */
	void on_touch_action(touch_action_t touch_action, const graphics::position_t &position);
	
	/** \brief Сообщение от клавиатуры или кнопок (клавиша/кнопка нажата)
	 */
	bool on_key_down(keycode_t keycode);
	
	/** \brief Сообщение от клавиатуры или кнопок (клавиша/кнопка отжата)
	 */
	bool on_key_up(keycode_t keycode);
	
private:
	static Application* m_instance;
	utils::task_queue_sptr m_task_queue;	///< Очередь задач игрового потока
	bool m_paused;							///< Признак приостановки работы приложения
	bool m_stopped;							///< Признак остановки работы приложения
	utils::task_wptr_t m_task_cache_clean;	///< Задача на периодическую очистку кэша
	
	graphics::GContainerSPtr m_scene;
	graphics::GRectangleSPtr m_rect;
	graphics::GImageSPtr m_img;
	graphics::GImageSPtr m_img2;
	utils::task_wptr_t m_task;
	void rotate_rect();
	
	Application();
	
	~Application();
	
	/** \brief Выполнить очистку кэша
	 */
	void clean_cache();
};

inline void app_create() { Application::create(); }

inline void app_destroy() { Application::destroy(); }

inline bool app_alive() { return Application::alive(); }

inline Application& app() { return Application::instance(); }

#endif // APPLICATION_HPP
