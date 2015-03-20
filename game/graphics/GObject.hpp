#ifndef GOBJECT_HPP
#define GOBJECT_HPP

#include <graphics/types.hpp>
#include <graphics/forwards.hpp>
#include <graphics/Renderer.hpp>
#include <utils/spin_lock.hpp>


namespace graphics
{
	
	class GObject
	{
	private:
		friend class ObjectsContainer;
		
	public:
		GObject();
		
		virtual ~GObject();
		
		/** \brief Получить владельца объекта
		 */
		inline ObjectsContainerPtr owner() { return m_owner; }
		
		/** \brief Установить видимость объекта
		 */
		void set_visible(bool value);
		
		/** \brief Получить видимость объекта
		 */
		inline bool visible() const { return m_visible; }
		
		/** \brief Установить цвет объекта
		 */
		void set_color(const color_t &value);
		
		/** \brief Получить цвет объекта
		 */
		inline const color_t& color() const { return m_color; }
		
		/** \brief Установить уровень непрозрачности объекта
		 */
		void set_opacity(opacity_t value);
		
		/** \brief Получить уровень непрозрачности объекта
		 */
		inline opacity_t opacity() const { return m_color.a; }
		
		/** \brief Установить центр трансформации
		 */
		void set_transform_point(const position_t &value);
		
		/** \brief Получить центр трансформации
		 */
		inline const position_t& transform_point() const { return m_transform_point; }
		
		/** \brief Установить вращение объекта
		 */
		void set_rotation(const rotation_t &value);
		
		/** \brief Получить вращение объекта
		 */
		inline const rotation_t& rotation() const { return m_rotation; }
		
		/** \brief Установить масштаб объекта
		 */
		void set_scale(const scale_t &value);
		
		/** \brief Получить масштаб объекта
		 */
		inline const scale_t& scale() const { return m_scale; }
		
		/** \brief Отрисовать объект
		 * WARNING Не допускается вызывать данный метод из какого-либо потока, кроме графического!
		 */
		void draw();
		
	protected:
		utils::spin_lock m_lock;				///< Защита атрибутов объекта
		ObjectsContainerPtr m_owner;			///< Владелец объекта
		ShaderProgramSPtr m_program;			///< Текущая шейдерная программа
		color_t m_color;						///< Цвет объекта
		position_t m_transform_point;			///< Центр трансформации объекта
		rotation_t m_rotation;					///< Вращение объекта
		scale_t m_scale;						///< Масштаб объекта
		
		/** \brief Выполнитть действия перед отрисовкой
		 */
		virtual void before_draw();
		
		/** \brief Выбрать и подготовить шейдерную программу
		 */
		virtual void set_program();
		
		/** \brief Выполнитть действия после отрисовки
		 */
		virtual void after_draw();
		
		/** \brief Выполнить отрисовку объекта
		 * Реализуется в наследованном классе.
		 */
		virtual void do_draw() = 0;
		
		/** \brief Отправить сигнал об изменении графики
		 */
		virtual void invalidate();
		
	private:
		bool m_visible;							///< Видимость объекта
	};
	
}

#endif // GOBJECT_HPP
