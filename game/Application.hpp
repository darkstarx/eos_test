#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <graphics/types.hpp>
#include <graphics/forwards.hpp>
#include <graphics/events.hpp>


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
	
	/** \brief Обработать создание отрисовщика
	 * WARNING Вызов метода не гарантируется, так как приложение может быть создано позднее,
	 * чем был создан отрисовщик.
	 * NOTE Чаще всего приложение создается раньше, чем отрисовщик.
	 * NOTE Метод выполняется в графическом потоке.
	 */
	void on_renderer_created();
	
	/** \brief Обработать разрушение отрисовщика
	 * WARNING Вызов метода не гарантируется, так как приложение может быть разрушено раньше,
	 * чем будет разрушен отрисовщик.
	 * NOTE В Android при закрытии приложения, оно разрушается раньше, чем отрисовщик.
	 * NOTE Метод выполняется в графическом потоке.
	 */
	void on_renderer_destroyed();
	
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
	bool m_paused;					///< Признак приостановки работы приложения
	bool m_stopped;					///< Признак остановки работы приложения
	
	graphics::GBoxSPtr m_box;
	
	Application();
	
	~Application();
};

inline void app_create() { Application::create(); }

inline void app_destroy() { Application::destroy(); }

inline bool app_alive() { return Application::alive(); }

inline Application& app() { return Application::instance(); }

#endif // APPLICATION_HPP
