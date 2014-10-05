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
	/** \brief ������� ��������
	 * 
	 * \param hDC Device context, ��������� ��� ������������� OpenGL.
	 * \param size ������� ����������� ���������.
	 */
	Renderer(HDC hDC, const dimension_t &size);
	~Renderer();

	/** \brief ��������� ����������� �����
	 */
	void start();

	/** \brief ���������� ����������� �����
	 */
	void stop();

private:
	HDC m_hDC;								///< ������������� ����������� ��������� (device context)
	dimension_t m_size;						///< ������� ����������� �����������
	std::atomic<bool> m_working;			///< ������� ���������� ������������ ������
	std::shared_ptr<std::thread> m_thread;	///< ������ ���������� ����������� �������

	/** \brief ��������� ���������� ���������� � ��������� ���� ����������
	 * ��������� ������������� ������� � ����������, ����� ������������� ������� ��������� ������ ����������,
	 * ����� ��������� ���� ���������� � ������������� fps, �� �������� ��� ����������.
	 * ����������� � ��������� ������ �� ��� ���, ���� ����������� ���� �� ����� �������� �� ��������� ������,
	 * ���������� �������� \� Renderer.
	 */
	void work();
};

#endif // RENDERER_H