//#ifndef FONTLAYOUT_HPP_
//#define FONTLAYOUT_HPP_
//
//#include <graphics/types.hpp>
//#include <graphics/forwards.hpp>
//#include <freetype/types.hpp>
//#include <map>
//
//
//namespace graphics
//{
//	/** @brief Вспомогательный класс, позволяющий строить строки глифов и выравнивать их по заданным параметрам
//	 * Обрабатывает текст, выполняя геометрическую расстановку глифов для каждого символа текста. Выравнивает
//	 * построенные строки текста по ширине и высоте текстового поля. При необходимости выполняет переносы строк
//	 * по словам или по символам (если слово не помещается в ширину текстового поля) и обрезку текста.
//	 * На основании строк глифов создаются регионы для @c GImageSubArray.
//	 */
//	class FontLayout
//	{
//	public:
//		/** @brief Данные глифа
//		 * Включают в себя только рект глифа в метке и рект глифа в текстуре шрифта.
//		 */
//		struct glyph_data_t
//		{
//			rectangle_t label_pos;		///< Рект глифа в метке (на экране, относительно начала координат метки)
//			rectangle_t texture_pos;	///< Рект глифа в текстуре шрифта (в атласе)
//			int advance;
//			int kerning;
//			rgba_t color;				///< Цвет глифа в метке. Актуально только для многоцветных меток.
//			freetype::char_t unicode;
//			glyph_data_t(const rectangle_t &label_pos, const rectangle_t &texture_pos, int advance, int kerning, const rgba_t &color, freetype::char_t unicode)
//			: label_pos(label_pos), texture_pos(texture_pos), advance(advance), kerning(kerning), color(color), unicode(unicode) {}
//		};
//
//		/** @brief Список глифов
//		 * Обязательно должен быть вектором, чтобы можно было индексировать глифы по порядку следования.
//		 */
//		typedef std::vector<glyph_data_t> glyphs_list_t;
//
//		/** @brief Карта границ слов
//		 * Список слов, представляющих собой пары <начало слова>,<конец слова>.
//		 * @param _Key индекс глифа, являющегося первым символом слова
//		 * @param _Tp индекс глифа, являющегося последним символом слова
//		 */
//		typedef std::map<int, int> words_edges_t;
//		
//		/** @brief Строка глифов
//		 * Содержит список глифов, входящих в строку, а также размеры строки в пикселах. Позиции
//		 * глифов в строке относительны и зависят от позиции строки, хранящейся в данной структуре.
//		 * Позиции и размеры строки используются при выравнивании и подсчете границ (@c rectangle)
//		 * и очертаний (@c bounding_rectangle) текста.
//		 */
//		struct glyphs_line_t
//		{
//			friend class FontLayout;
//			glyphs_line_t(const rectangle_t &rect) : m_rect(rect), m_last(true), m_full(true), m_trimmed(0) {}
//			inline bool empty() const { return m_glyphs.empty(); }
//			inline size_t size() const { return m_glyphs.size(); }
//			inline const rectangle_t& rectangle() const { return m_rect; }
//			inline const rectangle_t& bounding_rectangle() const { return m_bound_rect; }
//			inline const glyphs_list_t& glyphs() const { return m_glyphs; }
//		private:
//			rectangle_t m_rect;			///< границы строки текста
//			rectangle_t m_bound_rect;	///< очертания строки текста
//			position_t m_last_pen_pos;	///< позиция пера последнего символа
//			glyphs_list_t m_glyphs;		///< набор глифов, входящих в строку текста
//			words_edges_t m_words;		///< границы слов
//			bool m_last;				///< признак последней строки в параграфе
//			bool m_full;				///< признак заполненности строки
//			int m_trimmed;				///< кол-во невидимых глифов, обрезанных с конца строки
//			inline void push_back(const glyph_data_t &glyph_data) { m_glyphs.push_back(glyph_data); }
//		};
//
//		/** @brief Список линий глифов
//		 * Так представляется текст в метке @c GLabel.
//		 */
//		typedef std::list<glyphs_line_t> glyphs_lines_t;
//
//	public:
//		/** @brief Конструктор
//		 * @param font Шрифт, который используется для отображения текста.
//		 * @param line_spacing Межстрочный интервал (задается относительно высоты текста и используется.
//		 * @param load_glyphs Нужно ли загружать изображения глифов. По умолчанию true.
//		 *   для определения вертикального расстояния между базовыми линиями), фактически множитель высоты строки.
//		 */
//		FontLayout(const FontSPtr &font, const GLfloat line_spacing, const GLfloat char_spacing = 1.0f, bool load_glyphs = true);
//		
//		~FontLayout();
//
//		/** @brief Выполнить обработку текста и создать наборы глифов
//		 * Функция выполняет расстановку глифов из шрифта для отрисовки в метку, записывая их данные в
//		 * @c glyphs_lines.
//		 */
//		void process(const dimension_t &text_dimension, const std::string &text, bool clip, bool multiline);
//
//		/** @brief Выполнить обработку текста и создать наборы разноцветных глифов
//		 * Функция выполняет расстановку глифов из шрифта для отрисовки в метку, записывая их данные в
//		 * @c glyphs_lines, а также помечает глифы, попавшие в цветовой регион, соответствующим цветом.
//		 */
//		void process(const dimension_t &text_dimension, const std::string &text, bool clip, bool multiline, const color_regions_t &colors, const rgba_t &default_color);
//
//		/** @brief Выполнить выравнивание глифов, созданных после обработки текста
//		 * Вызывает горизонтальное выравнивание каждой строки по ширине и вертикальное выравнивание всех строк по высоте.
//		 * NOTE Предполагается отсутствие исходного выравнивания, поэтому может быть вызван только один раз после обработки текста.
//		 */
//		void process_alignment(const dimension_t &text_dimension, const horizontal_alignment_t h_align, const vertical_alignment_t v_align);
//
//		/** @brief Выполнить горизонтальное выравнивание строки @param glyphs_line по ширине
//		 * NOTE Предполагается отсутствие исходного выравнивания, поэтому может быть вызван только один раз после обработки текста.
//		 */
//		void process_h_alignment(const dimension_t &text_dimension, const horizontal_alignment_t h_align, glyphs_line_t &glyphs_line);
//
//		/** @brief Выполнить вертикальное выравнивание всех строк по высоте
//		 * NOTE Предполагается отсутствие исходного выравнивания, поэтому может быть вызван только один раз после обработки текста.
//		 */
//		void process_v_alignment(const dimension_t &text_dimension, const vertical_alignment_t v_align);
//
//		/** @brief Получить размеры текста без учета обрезки, переносов и выравнивания
//		 * Функция выполняет расстановку глифов из шрифта для определения размеров текстового поля, записывая данные глифов
//		 *   в @c glyphs_lines.
//		 * NOTE Это упрощенный метод расстановки глифов в строки, предназначенный только для замера метрик. Чтобы определить
//		 * метрики текста с учетом обрезки, переносов и выравнивания, следует использовать функции @c process,
//		 *   @c process_alignment и получать размеры методами @c rectangle или @c bounding_rectangle.
//		 * @param text Текст, размеры и положение которого следует определить.
//		 * @return Размеры и положение поля, являющегося очертанием данного текста.
//		 */
//		rectangle_t measure_text(const std::string &text);
//
//		/** @brief Получить набор строк с глифами, построенный при обработке текста методом @c process
//		 */
//		inline const glyphs_lines_t& glyphs_lines() { return m_lines; }
//
//		/** @brief Получить границы текста
//		 */
//		rectangle_t rectangle();
//
//		/** @brief Получить очертания (видимые границы) текста
//		 */
//		rectangle_t bounding_rectangle();
//
//	private:
//		const FontSPtr m_font;				///< Используемый шрифт
//		const GLfloat m_line_spacing;		///< Коэффициент для вычисления высоты строки
//		const GLfloat m_char_spacing;		///< Коэффициент для вычисления смещения пера
//		GLfloat m_xheight;					///< Высота глифа x (испрользуется при вертикальном выравнивании)
//		glyphs_lines_t m_lines;				///< Набор строк глифов, подготовленный в результате обработки текста
//		const bool m_load_glyphs;			///< Признак необходимости загружать изображения глифов
//		bool m_xheight_inited;				///< Значение x-height уже определено
//		
//		/** @brief Получить x-height
//		 * Возвращает высоту глифа "x", если глиф успешно получен от freetype, или 0.0f в противном случае.
//		 * Возводит флаг m_xheight_inited, если значение x-height успешно определено.
//		 */
//		GLfloat xheight();
//	};
//
//} /* namespace graphics */
//
//#endif /* FONTLAYOUT_HPP_ */
