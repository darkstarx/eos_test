#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <graphics/types.hpp>
#include <graphics/Renderer.hpp>
#include <graphics/events.hpp>


class Application
{
public:
	static void create();
	
	static void destroy();
	
	inline static Application* instance() { return m_instance; }
	
	void create_renderer();
	
	void destroy_renderer();
	
	inline graphics::Renderer* renderer() { return m_renderer; }
	
	/** @brief Таймер приложения
	 * Метод вызывается раз в 60 секунд в основном потоке приложения, позволяет выполнять
	 * очень кратковременные периодические операции.
	 * WARNING Нельзя задерживать выполнение этого метода, так как он выполняется в том же потоке,
	 * что и обработчики событий ввода пользовательских команд!
	 */
	void tick();
	
	/** @brief Запуск приложения
	 * Метод выполняется при запуске приложения.
	 */
	void start();
	
	/** @brief Останов приложения
	 * Метод выполняется при остановке приложения.
	 */
	void stop();
	
	/** @brief Приостановка приложения
	 * Метод выполняется при временной приостановке приложения.
	 */
	void pause();
	
	/** @brief Возобновление работы приложения
	 * Метод выполняется при возобновлении работы приложения.
	 */
	void resume();
	
	/** @brief Сообщение от манипулятора (тачпада или мыши)
	 */
	void on_touch_action(touch_action_t touch_action, const graphics::position_t &position);
	
	/** @brief Сообщение от клавиатуры или кнопок (клавиша/кнопка нажата)
	 */
	bool on_key_down(keycode_t keycode);
	
	/** @brief Сообщение от клавиатуры или кнопок (клавиша/кнопка отжата)
	 */
	bool on_key_up(keycode_t keycode);
	
private:
	static Application* m_instance;
	graphics::Renderer* m_renderer;
	
	Application();
	
	~Application();
};


inline Application * app() { return Application::instance(); }

#endif // APPLICATION_HPP
