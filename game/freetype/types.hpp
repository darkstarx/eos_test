#ifndef FREETYPE_TYPES_HPP
#define FREETYPE_TYPES_HPP

#include <stdint.h>
#include <string>


namespace freetype
{
	
	/** \brief ���, ������������ ��� unicode ������������� �������.
	 */
	typedef int32_t char_t;
	
	/** \brief ��� ������� �����
	 */
	enum class glyph_outline_t {
		none,	///< ������� �������� �����������, ������� ����
		in,		///< ������������ ��������� �����, �������� ������� �������
		out,	///< ������������ ������ ���� ������� ��������, ��� ���� �����
		full	///< ������������ ���� � ������� ��������
	};
	
}

namespace utils
{
	
	std::string to_string(const freetype::glyph_outline_t &v);
	
}

#endif // FREETYPE_TYPES_HPP
