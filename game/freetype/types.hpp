#ifndef FREETYPE_TYPES_HPP
#define FREETYPE_TYPES_HPP

#include <stdint.h>
#include <string>


namespace freetype
{
	
	/** \brief Тип, используемый для unicode представления символа.
	 */
	typedef int32_t char_t;
	
	/** \brief Тип обводки глифа
	 */
	enum class glyph_outline_t {
		none,	///< Обводка контуров отсутствует, обычный глиф
		in,		///< Отображается внутрення часть, исключая обводку контура
		out,	///< Отображается только сама обводка контуров, без тела глифа
		full	///< Отображается глиф и обводка контуров
	};
	
}

namespace utils
{
	
	std::string to_string(const freetype::glyph_outline_t &v);
	
}

#endif // FREETYPE_TYPES_HPP
