#ifndef FONTS_FONT_HPP
#define FONTS_FONT_HPP

#include <fonts/types.hpp>
#include <freetype/forwards.hpp>
#include <freetype/types.hpp>
#include <graphics/forwards.hpp>
#include <utils/bytearray.hpp>
#include <utils/functional.hpp>
#include <utils/log.hpp>
#include <resources/forwards.hpp>
#include <string>
#include <map>


namespace graphics
{
	
	typedef std::function<freetype::FTFaceSPtr()> reload_font_f;
	
	/** \brief Класс работы со шрифтом
	 */
	class Font
	{
	public:
		/** \brief Сведения о глифе шрифта
		 */
		struct glyph_t
		{
			rectangle_t rect;				///< размер и позиция изображения глифа в атласе (размеры могут быть заданы, даже если глиф не в атласе)
			bool has_bitmap;				///< признак того, что глиф должен содержаться в атласе шрифта
			bool loaded;					///< признак того, что глиф загружен в атлас шрифта
			bool valid;						///< признак корректности глифа (если фритайп успешно вернул данные глифа)
			int bearing_left;				///< горизонтальное смещение левой границы глифа от положения пера вправо
			int bearing_top;				///< вертикальное смещение верхней границы глифа от базовой линии вверх
			int advance;					///< смещение пера для позиционирования следующего глифа
			glyph_t() : has_bitmap(false), loaded(false), valid(true), bearing_left(0), bearing_top(0), advance(0) {}
			inline bool is_visible() const { return rect.w > 0.0f && rect.h > 0.0f; }
		};
		
	private:
		/** \internal Карта описателей глифов
		 * Ключ - unicode символа. Значение - глиф и его позиция в текстуре, в пикселях.
		 */
		typedef std::map<freetype::char_t, glyph_t> glyphs_t;
		
	public:
		/** Построение на основе данных уже загруженного шрифта.
		 * \param face Объект шрифта FreeType.
		 * \param reload_font функтор перезагрузки шрифта.
		 * \param source Имя исходных данных шрифта (например, может быть две ttf-ки одного и того же семейства).
		 * \param outline Тип обводки глифа.
		 * \param outline_width Толщина обводки в пикселах.
		 */
		explicit Font(const freetype::FTFaceSPtr &face, const reload_font_f &reload_face,
			const std::string &source, freetype::glyph_outline_t outline = freetype::glyph_outline_t::none,
			float outline_width = 0.0f
		);
		
		~Font();
		
		/** \brief Получить размер шрифта (кегль)
		 */
		int size() const { return m_size; }
		
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
		
		/** \brief Запретить выгрузку шрифта
		 */
		void lock_unload(); 
		
		/** \brief Разрешить выгрузку шрифта
		 */
		void unlock_unload();
		
		/** \brief Получить признак возможности выгрузки шрифта
		 * (use_count == 0)
		 * \return true, если шрифт может быть выгружен; false, если выгрузка шрифта запрещена.
		 */
		bool unloading_unlocked() const;
		
		/** \brief Получить признак того, что шрифт выгружен
		 */
		bool is_unloaded() const;
		
		/** \brief Выгрузить шрифт
		 */
		void unload();
		
		/** \brief Перезагрузить шрифт
		 */
		void reload();
		
		/** \brief Получить глиф
		 * Данная функция возвращает глиф шрифта и его позицию в текстуре по его unicode.
		 * \param unicode Юникод символа, для которого следует загрузить глиф.
		 * \param need_bitmap Если true, то глиф загружается вместе с битмапом, который помещается в атлас и
		 *   может использоваться для отрисовки текста; если false, то возвращаемый результат может быть
		 *   использован только для операций с метриками глифа (битмап не загружается и в атлас шрифта не
		 *   попадает).
		 */
		glyph_t get_glyph(const freetype::char_t unicode, bool need_bitmap = true);
		
		/** \brief Получить горизонтальный кернинг для символов \c char1 и \c char2
		 * Читает из шрифта горизонтальный кернинг для пары глифов, соответствующих символам \c char1 и \c char2, и
		 * возвращает в качестве результата значение кернинга в пикселах.
		 * \param char1 Юникод символ слева (для горизонтального расположения текста).
		 * \param char2 Юникод символ справа (для горизонтального расположения текста).
		 */
		int get_offset(const freetype::char_t char1, const freetype::char_t char2);
		
		/** \brief Получить текстуру с атласом шрифта
		 */
		inline const graphics::TextureSPtr& texture() const { return m_texture; }
		
		/** \brief Получить используемую шрифтом фабрику
		 */
		inline resources::Factory* factory() const { return m_factory; }
		
		/** \brief Получить морду шрифта
		 */
		inline freetype::FTFaceSPtr face() const { return m_face; }
		
		/** \brief Получить функтор перезагрузки морды шрифта
		 */
		inline reload_font_f reload_func() const { return m_reload_face; }
		
		/** \brief Получить тип обводки глифов
		 */
		inline freetype::glyph_outline_t outline() const { return m_outline; }
		
		/** \brief Получить ширину обводки глифов
		 */
		inline float outline_width() const { return m_outline_width; }
		
		/** \brief Получить имя источника шрифта
		 */
		inline const std::string& source() { return m_source; }
		
	private:
		const freetype::glyph_outline_t m_outline;	///< Тип обводки глифов
		const float m_outline_width;				///< Толщина обводки глифов
		const std::string m_source;					///< Источник, из которого был взят шрифт (например, файл ttf)
		resources::Factory *m_factory;				///< Фабрика
		int m_size;									///< Размер шрифта
		reload_font_f m_reload_face;				///< Функтор перезагрузки шрифта
		int m_use_count;							///< Счётчик использования 
		freetype::FTFaceSPtr m_face;				///< Обёртка над API FreeType
		graphics::TextureSPtr m_texture;			///< Атлас глифов
		glyphs_t m_glyphs;							///< Уже созданные глифы
		position_t m_next_glyph_pos;				///< Позиция следующего глифа в текстуре
		
		/** Максимальная высота символа в строке, располагаемой в текстуре. Необходима, когда начинаем формировать
		 * в текстуре новую строку глифов.
		 */
		GLfloat m_max_height_in_line;
		
		/** \brief Загрузить даные шрифта
		 */
		void load_font_data();
		
		/** \brief Выгрузить данные шрифта
		 */
		void unload_font_data();
		
		/** \brief Получить размеры для атласа
		 */
		int atlas_size();
		
		/** Обёртка над glTexSubImage2D */
		static bool load_bitmap_to_texture(const Texture2DSPtr &texture, const rectangle_t &texture_rect, const utils::bytearray &data);
		
		static Texture2DSPtr make_empty_font_texture(const resources::Factory *factory, int size);
	};
}

#endif  // FONTS_FONT_HPP
