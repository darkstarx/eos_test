#ifndef FREETYPE_FTFACE_HPP
#define FREETYPE_FTFACE_HPP

#include <freetype/types.hpp>
#include <freetype/forwards.hpp>
#include <utils/forwards.hpp>
#include <ft2build.h>
#include <freetype.h>
#include <string>


namespace freetype
{
	
	class FTFace
	{
	public:
		FTFace(const utils::bytearray_sptr_t &font_data, const unsigned int font_size);
		
		~FTFace();
		
		/** \brief Получить глиф символа по его коду
		 * \param unicode Код символа.
		 * \param need_bitmap Флаг, нужен ли bitmap для символа. Иногда нам требуются только параметры глифов,
		 * вроде ascender и advance, в таком случае для оптимизации можно установить этот флаг в false.
		 * \param outline_type Тип обводки.
		 * \param outline_width Толщина обводки в пикселах.
		 * \return Объект глифа. Может вернуть нулевой указатель, если произошла ошибка загрузки шрифта! Обязательно
		 * проверять результат перед использованием!
		 */
		FTGlyphSPtr get_glyph(const char_t unicode, const bool need_bitmap, glyph_outline_t outline_type = glyph_outline_t::none, float outline_width = 0.0f) const;
		
		/** \brief Получить размер шрифта
		 */
		inline unsigned int size() const { return m_font_size; };
		
		/** \brief Получить описание шрифта
		 */
		std::string description() const;
		
		/** \brief Получить кернинг
		 * Возвращает смещение (корректировку advance) между двумя символами.
		 */
		int get_offset(const freetype::char_t char1, const freetype::char_t char2);
		
		/** \brief Получить ассендер шрифта
		 */
		FT_Pos ascender() const;
		
		/** \brief Получить десендер шрифта
		 */
		FT_Pos descender() const;
		
		/** \brief Получить высоту шрифта
		 */
		inline FT_Pos font_height() const { return m_font_height; }
		
		/** \brief Получить признак валидности шрифта
		 * Шрифт невалиден, если не удалось его загрузить из FreeType. В этом случае экземпляр FTFace будет возвращать
		 * пустые данные.
		 */
		inline bool valid() const { return m_face; };
		
	private:
		const utils::bytearray_sptr_t m_font_data;		///< Содержимое файла шрифта
		const FTLibrarySPtr m_library;					///< Объект библиотеки freetype
		const int m_font_size;							///< Размер шрифта
		FT_Pos m_font_height;							///< Высота строки текста
		FT_Face m_face;									///< Данные шрифта
	};
	
}

#endif // FREETYPE_FTFACE_HPP
