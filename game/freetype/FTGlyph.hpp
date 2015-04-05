#ifndef FREETYPE_FTGLYPH_HPP
#define FREETYPE_FTGLYPH_HPP

#include <freetype/types.hpp>
#include <freetype/forwards.hpp>
#include <resources/forwards.hpp>
#include <utils/bytearray.hpp>
#include <utils/string_utils.hpp>
#include <ft2build.h>
#include <freetype.h>
#include <ftglyph.h>


namespace freetype
{
	
	class FTGlyph
	{
	public:
		FTGlyph(const FT_Face &face, const char_t unicode, bool need_bitmap);
		
		~FTGlyph();
		
		/** \brief Получить битмап глифа
		 * Создает битмап и заполняет его данными из глифа, если они есть, иначе возвращает пустой указатель.
		 */
		resources::ImageSPtr get_biamap();
		
		/** \brief Получить горизонтальное смещение пера для отрисовки следующего символа
		 * Определен в пикселах, даже если битмап не загружается.
		 */
		inline FT_Pos advance() const { return m_advance; }
		
		/** \brief Получить горизонтальное смещение левой границы глифа (вправо - положительное) от положения пера
		 * Определен в пикселах, даже если битмап не загружается.
		 */
		inline FT_Int bearing_left() const { return m_bearing_left; }
		
		/** \brief Получить вертикальное смещение верхней точки глифа (вверх - положительное) от базовой линии
		 * Определен в пикселах, даже если битмап не загружается.
		 */
		inline FT_Int bearing_top() const { return m_bearing_top; }
		
		/** \brief Получить ширину глифа
		 * Определен в пикселах, даже если битмап не загружается.
		 */
		inline FT_Int width() const { return m_width; }
		
		/** \brief Получить высоту глифа
		 * Определен в пикселах, даже если битмап не загружается.
		 */
		inline FT_Int height() const { return m_height; }
		
		/** \brief Получить признак видимости глифа
		 * Функция определяет, является ли символ грифа управляющим. Управляющие символы не имеют отображения, для
		 * них отсутствует размер.
		 */
		inline bool is_control() const { return utils::string_utils::is_control(m_unicode); }
		
	private:
		char_t m_unicode;		///< Код символа
		FT_Glyph m_glyph;		///< Данные глифа
		FT_Pos m_advance;		///< Смещение пера для следующего глифа
		FT_Int m_width;			///< Ширина глифа
		FT_Int m_height;		///< Высота глифа
		FT_Int m_bearing_left;	///< Горизонтальное смещение от позиции пера до начала глифа.
		FT_Int m_bearing_top;	///< Вертикальное смещение от позиции пера (базовой линии) до верхней точки глифа.
	};
	
}

#endif // FREETYPE_FTGLYPH_HPP
