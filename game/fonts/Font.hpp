#ifndef FONTS_FONT_HPP
#define FONTS_FONT_HPP

#include <freetype/forwards.hpp>
#include <freetype/types.hpp>
#include <fonts/types.hpp>
#include <graphics/forwards.hpp>
#include <utils/forwards.hpp>
#include <map>


namespace fonts
{
	
	class Font
	{
	private:
		/** \brief Карта глифов
		 * Ключ - юникод, значение - данные глифа.
		 */
		typedef std::map<freetype::char_t, glyph_t> glyphs_map_t;
		
		/** \brief Глифы шрифта
		 * Карта и атлас глифов, а также позиция для следующего глифа, который будет загружен в
		 * атлас.
		 */
		struct glyphs_t {
			glyphs_map_t map;						///< Карта глифов
			graphics::TextureSPtr atlas;			///< Атлас глифов
			graphics::position_t next_glyph_pos;	///< Позиция для следующего глифа в атласе
			float curr_line_height;					///< Высота текущей строки глифов в атласе
		};
		
	public:
		Font(const freetype::FTFaceSPtr &face);
		
		~Font();
		
		/** \brief Получить размер шрифта (кегль)
		 */
		unsigned int size() const { return m_size; }
		
		/** \brief Получить высоту шрифта
		 * Высота шрифта используется для подсчёта межстрочного интервала.
		 */
		int font_height() const;
		
		/** \brief Получить ассендер шрифта
		 * Возвращает ассендер шрифта - расстояние между базовой линией и верхней точкой горизонтально расположенного текста.
		 */
		int ascender() const;
		
		/** \brief Получить десендер шрифта
		 * Возвращает десендер шрифта - расстояние между базовой линией и нижней точкой горизонтально расположенного текста.
		 */
		int descender() const;
		
		/** \brief Получить размер текстуры
		 */
		size_t texture_size() const;
		
		/** \brief Получить признак того, что атлас шрифта загружен
		 */
		bool is_loaded() const;
		
		/** \brief Выгрузить шрифт
		 */
		void unload();
		
		/** \brief Перезагрузить шрифт
		 */
		void reload();
		
		/** \brief Получить глиф
		 * Возвращает глиф шрифта, соответствующий коду символа.
		 * \param unicode Код символа, для которого следует загрузить глиф.
		 * \param need_bitmap Если true, то глиф загружается вместе с битмапом, который помещается
		 * в атлас глифов и может использоваться для отрисовки символа; если false, то возвращаемый
		 * результат может быть использован только для операций с метриками глифа (битмап не
		 * загружается и в атлас шрифта не попадает).
		 */
		glyph_t get_glyph(const freetype::char_t unicode, bool need_bitmap = true);
		
		/** \brief Получить горизонтальный кернинг для символов \c char1 и \c char2
		 * Читает из шрифта горизонтальный кернинг для пары глифов, соответствующих символам \c char1 и \c char2, и
		 * возвращает в качестве результата значение кернинга в пикселах.
		 * \param char1 Юникод символ слева (для горизонтального расположения текста).
		 * \param char2 Юникод символ справа (для горизонтального расположения текста).
		 */
		int get_offset(const freetype::char_t char1, const freetype::char_t char2);
		
	private:
		freetype::FTFaceSPtr m_face;		///< Обёртка над API FreeType
		unsigned int m_size;				///< Размер шрифта
		glyphs_t m_glyphs;					///< Глифы
		
		/** \brief Получить размер для текстуры атласа
		 */
		unsigned int atlas_dimension();
		
		/** \brief Загрузить даные шрифта
		 */
		void load_font_data();
		
		/** \brief Выгрузить данные шрифта
		 */
		void unload_font_data();
		
		/** \brief Скопировать битмап в текстуру
		 */
		static void copy_bitmap_to_texture(const graphics::TextureSPtr &texture, const graphics::rectangle_t &rect, const utils::bytearray &data);
	};
	
}

#endif // FONTS_FONT_HPP
