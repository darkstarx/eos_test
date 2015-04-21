#ifndef FONTS_FORWARDS_HPP
#define FONTS_FORWARDS_HPP

#include <graphics/types.hpp>


namespace fonts
{
	
	/** \brief Данные глифа
	 * Содержит сведения о расположении глифа в атласе, загружен ли он в атлас и имеет ли вообще изображение,
	 * а также валидность (глиф содержится в шрифте), отступы и адванс.
	 */
	struct glyph_t
	{
		glyph_t()
		: has_bitmap(false)
		, loaded(false)
		, valid(true)
		, bearing_left(0)
		, bearing_top(0)
		, advance(0)
		{}
		
		inline bool is_visible() const
		{ return rect.w > 0.0f && rect.h > 0.0f; }
		
		graphics::rectangle_t rect;		///< Область глифа в атласе шрифта
		bool has_bitmap;				///< Признак того, что глиф должен содержаться в атласе шрифта
		bool loaded;					///< Признак того, что глиф загружен в атлас шрифта
		bool valid;						///< Признак корректности глифа (если фритайп успешно вернул данные глифа)
		int bearing_left;				///< Горизонтальное смещение левой границы глифа от положения пера вправо
		int bearing_top;				///< Вертикальное смещение верхней границы глифа от базовой линии вверх
		int advance;					///< Смещение пера для позиционирования следующего глифа
	};
	
}

#endif // FONTS_FORWARDS_HPP
