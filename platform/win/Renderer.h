#ifndef RENDERER_H
#define	RENDERER_H

#include <windows.h>
#include <atomic>
#include <thread>
#include "game/Definiton.h"
#include <memory>


class Renderer
{
public:
	/** \brief Создать рендерер
	 * 
	 * \param hDC Device context, требуется для инициализации OpenGL.
	 * \param size Размеры поверхности отрисовки.
	 */
	Renderer(HDC hDC, const dimension_t &size);
	~Renderer();

	/** \brief Запустить графический поток
	 */
	void start();

	/** \brief Остановить графический поток
	 */
	void stop();

private:
	HDC m_hDC;								///< идентификатор поверхности отрисовки (device context)
	dimension_t m_size;						///< размеры поверхности отображения
	std::atomic<bool> m_working;			///< признак активности графического потока
	std::shared_ptr<std::thread> m_thread;	///< объект управления графическим потоком

	/** \brief Выполнить подготовку приложения и запустить цикл рендеринга
	 * Выполняет инициализацию графики и приложения, после инициализации графики запускает потоки приложения,
	 * затем запускает цикл рендеринга с установленным fps, не допуская его превышения.
	 * Выполняется в отдельном потоке до тех пор, пока разделяемый флаг не будет выключен из основного потока,
	 * владеющего объектом \с Renderer.
	 */
	void work();
};

#endif // RENDERER_H