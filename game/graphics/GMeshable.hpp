#ifndef GMESHABLE_H
#define GMESHABLE_H

#include <graphics/GObject.hpp>


namespace graphics
{
	
	class GMeshable : public GObject
	{
	public:
		enum class mesh_type_e {
			mt_2d,
			mt_3d
		};
		
	private:
		typedef GObject super;
		
	public:
		GMeshable(mesh_type_e mesh_type);
		
		virtual ~GMeshable();
		
	protected:
		std::shared_ptr<GLfloat> m_vertices;	///< Буфер вершин
		std::shared_ptr<GLushort> m_indices;	///< Буфер индексов
		GLsizei m_vertices_count;				///< Кол-во элементов в буфере вершин
		GLsizei m_indices_count;				///< Кол-во элементов в буфере индексов
		
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
		
	protected:
		/** \brief Выполнитть действия перед отрисовкой
		 */
		virtual void before_draw();
		
		/** \brief Выбрать и подготовить шейдерную программу
		 */
		virtual void set_program();
		
	private:
		mesh_type_e m_mesh_type;			///< Тип сетки вершин (2d/3d)
		bool m_vertices_valid;				///< Признак актуальности буфера вершин
		bool m_indices_valid;				///< Признак актуальности буфера индексов
	};
	
}

#endif // GMESHABLE_H
