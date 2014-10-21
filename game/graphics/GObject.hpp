#ifndef GOBJECT_HPP
#define GOBJECT_HPP

#include <graphics/types.hpp>
#include <graphics/forwards.hpp>
#include <utils/spin_lock.hpp>


namespace graphics
{
	
	class GObject
	{
	public:
		GObject();
		
		virtual ~GObject();
		
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
		utils::spin_lock m_lock;				///< Объект синхронизации потоков
		ShaderProgramSPtr m_program;			///< Текущая шейдерная программа
		
		std::shared_ptr<GLfloat> m_vertices;	///< Буфер вершин
		std::shared_ptr<GLushort> m_indices;	///< Буфер индексов
		GLsizei m_vertices_count;				///< Кол-во элементов в буфере индексов
		GLsizei m_indices_count;				///< Кол-во элементов в буфере индексов
		color_t m_color;						///< Цвет объекта
		position_t m_transform_point;			///< Центр трансформации объекта
		rotation_t m_rotation;					///< Вращение объекта
		scale_t m_scale;						///< Масштаб объекта
		
		/** \brief Пометить буфер вершин не актуальными
		 */
		inline void invalidate_vertices() { m_vertices_valid = false; }
		
		/** \brief Пометить буфер индексов не актуальными
		 */
		inline void invalidate_indexes() { m_indices_valid = false; }
		
		/** \brief Обновить буфер вершин
		 */
		virtual void update_vertices();
		
		/** \brief Обновить буфер индексов
		 */
		virtual void update_indices();
		
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
		
	private:
		bool m_visible;						///< Видимость объекта
		bool m_vertices_valid;				///< Признак актуальности буфера вершин
		bool m_indices_valid;				///< Признак актуальности буфера индексов
	};
	
}

#endif // GOBJECT_HPP
