#ifndef GRAPHICS_GIMAGES_HPP
#define GRAPHICS_GIMAGES_HPP

#include <graphics/GMeshable.hpp>
#include <list>


namespace graphics
{
	
	class GImages : public GMeshable
	{
	private:
		typedef GMeshable super;
		
	public:
		GImages(const position_t& position);
		
		virtual ~GImages();
		
		/** \brief ���������� ������� �������
		 */
		void set_position(const position_t& value);
		
		/** \brief �������� ������� �������
		 */
		inline const position_t& position() { return m_position; }
		
		/** \brief �������� ������
		 */
		void add_region(const graphics::region_t& region);
		
		/** \brief ������� ��� �������
		 */
		void clear_regions();
		
		/** \brief ���������� �������� �����������
		 */
		void set_image(const graphics::TextureSPtr &texture);
		
		/** \brief ���������� �������� �����
		 */
		void set_mask(const graphics::TextureSPtr &texture);
		
	protected:
		std::shared_ptr<GLfloat> m_img_coords;	///< ���������� ���������� �����������
		std::shared_ptr<GLfloat> m_msk_coords;	///< ���������� ���������� �����
		GLsizei m_img_coords_count;				///< ���-�� ��������� � ������ ���������� ��������� �����������
		GLsizei m_msk_coords_count;				///< ���-�� ��������� � ������ ���������� ��������� �����
		
		/** \brief �������� ����� ���������� ��������� ����������� �� ����������
		 */
		inline void invalidate_img_coords() { m_img_coords_valid = false; }
		
		/** \brief �������� ����� ���������� ��������� ����� �� ����������
		 */
		inline void invalidate_msk_coords() { m_msk_coords_valid = false; }
		
		/** \brief �������� ����� ���������� ��������� �����������
		 */
		virtual void update_img_coords();
		
		/** \brief �������� ����� ���������� ��������� �����
		 */
		virtual void update_msk_coords();
		
	protected:
		/** \brief �������� ����� ������
		 */
		virtual void update_vertices();
		
		/** \brief �������� ����� ��������
		 */
		virtual void update_indices();
		
		/** \brief ���������� �������� ����� ����������
		 */
		virtual void before_draw();
		
		/** \brief ������� � ����������� ��������� ���������
		 */
		virtual void set_program();
		
		/** \brief ��������� ��������� �������
		 */
		virtual void do_draw();
		
	private:
		position_t m_position;					///< ������� �������
		std::list<region_t> m_regions;			///< ��������� ��������
		TextureSPtr m_tex_img;					///< �������� �����������
		TextureSPtr m_tex_msk;					///< �������� �����
		bool m_img_coords_valid;				///< ������� ������������ ������ ���������� ��������� �����������
		bool m_msk_coords_valid;				///< ������� ������������ ������ ���������� ��������� �����
	};
	
}

#endif // GRAPHICS_GIMAGES_HPP
