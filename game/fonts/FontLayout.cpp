//#include "FontLayout.hpp"
//#include <utils/calltrace.hpp>
//#include <freetype/types.hpp>
//#include <freetype/FTGlyph.hpp>
//#include <graphics/Font.hpp>
//#include <utils/log.hpp>
//#include <utils/round.hpp>
//#include <utils/StringUtils.hpp>
//#include <utf8-cpp/utf8.h>
//
//
//namespace graphics
//{
//	
//	FontLayout::FontLayout(const FontSPtr& font, const GLfloat line_spacing, const GLfloat char_spacing, bool load_glyphs)
//	: m_font(font)
//	, m_line_spacing(line_spacing)
//	, m_char_spacing(char_spacing)
//	, m_xheight(0.0f)
//	, m_load_glyphs(load_glyphs)
//	, m_xheight_inited(false)
//	{
//		CALLTRACE();
//	}
//	
//	FontLayout::~FontLayout()
//	{
//		CALLTRACE();
//	}
//	
//	void FontLayout::process(const dimension_t& text_dimension, const std::string& text, bool clip, bool multiline)
//	{
//		CALLTRACE();
//		process(text_dimension, text, clip, multiline, color_regions_t(), rgba_t());
//	}
//	
//	void FontLayout::process(const dimension_t &text_dimension, const std::string &text, bool clip,
//		bool multiline, const color_regions_t &colors, const rgba_t &default_color)
//	{
//		CALLTRACE();
//		
//		const std::string::const_iterator str_beg = text.begin();
//		const std::string::const_iterator str_end = text.end();
//		std::string::const_iterator char_current = str_beg;
//		const GLfloat font_height = m_font->font_height();
//		const GLfloat line_height = floor(m_line_spacing * font_height);
//		const int font_asc = m_font->ascender();
//		position_t pen_pos;
//		m_lines.clear();
//		
//		/** \internal Хелпер для различных обрезок строки глифов
//		 */
//		struct clipper_t
//		{
//			/** Конструктор */
//			clipper_t(const dimension_t &text_dimension, GLfloat char_spacing, int ascender)
//			: m_text_dimension(text_dimension)
//			, m_char_spacing(char_spacing)
//			, m_ascender(ascender)
//			{}
//			/** Подрезать строку для добавления многоточия.
//			 * \param pen_pos текущая позиция пера. Параметр inout.
//			 * \param line текущая строка глифов (линия), которую обрезаем. Параметр inout.
//			 * \param ellipsis данные глифа многоточия, помещённого в шрифт.
//			 * \return Если удалось обрезать строку так, что многоточие влезло в строку по ширине, возвращаем true.
//			 * Как правило, false возвращается, если ширина строки меньше ширины многоточия. */
//			bool ellipsize(position_t &pen_pos, glyphs_line_t &line, const Font::glyph_texture_data_t &ellipsis)
//			{
//				CALLTRACE();
//				bool bound_changed = false;
//				// Текущая правая граница елипсиса
//				GLfloat right_edge = pen_pos.x + ellipsis.bearing_left + ellipsis.rect.w;
//				// Пока не влезет глиф ellipsis, убираем последний символ из строки
//				while (right_edge > m_text_dimension.w && !line.empty()) {
//					const glyph_data_t &prev_glyph = line.m_glyphs.back();
//					pen_pos.x -= utils::round((prev_glyph.advance + prev_glyph.kerning) * m_char_spacing);
//					right_edge = pen_pos.x + ellipsis.bearing_left + ellipsis.rect.w;
//					line.m_glyphs.pop_back();
//					bound_changed = true;
//				}
//				// После обрезки необходимо пересчитать очертания строки
//				if (bound_changed) update_line_rect(line);
//				// Если многоточие умещается в ширине и высоте поля, то возвращаем возможность вставки
//				return (
//					m_text_dimension.w >= right_edge &&
//					m_text_dimension.h >= line.rectangle().y + m_ascender - ellipsis.bearing_top + ellipsis.rect.h
//				);
//			}
//			/** Подрезать строку, удалив невидимые глифы с конца строки.
//			 * Выполняет удаление глифов из переданной линии line, обновляя поля m_last_pen_pos, m_trimmed */
//			void trim_right(glyphs_line_t &line)
//			{
//				CALLTRACE();
//				if (line.empty()) return;
//				bool bound_changed = false;
//				while (!line.empty() && line.m_glyphs.back().label_pos.w == 0.0f) {
//					const glyph_data_t &glyph = line.m_glyphs.back();
//					line.m_last_pen_pos.x -= utils::round((glyph.advance + glyph.kerning) * m_char_spacing);
//					line.m_glyphs.pop_back();
//					line.m_trimmed += 1;
//					bound_changed = true;
//				}
//				// После обрезки необходимо пересчитать очертания строки
//				// Удаляли только пробелы, поэтому границы слов не пересчитываем
//				if (bound_changed) update_line_rect(line, false);
//			}
//			/** Подрезать строку, удалив определенное кол-во глифов с конца строки
//			 * \param line линия, из которой вырезать символы.
//			 * \param count количество глифов для обрезки. */
//			void trim_right(glyphs_line_t &line, size_t count)
//			{
//				CALLTRACE();
//				if (line.empty()) return;
//				const bool bound_changed = count > 0;
//				while (!line.empty() && count > 0) {
//					const glyph_data_t &glyph = line.m_glyphs.back();
//					line.m_last_pen_pos.x -= utils::round((glyph.advance + glyph.kerning) * m_char_spacing);
//					line.m_glyphs.pop_back();
//					--count;
//				}
//				// После обрезки необходимо пересчитать очертания строки
//				if (bound_changed) update_line_rect(line);
//			}
//		private:
//			const dimension_t m_text_dimension;
//			const GLfloat m_char_spacing;
//			const int m_ascender;
//			/** Обновить очертания строки глифов после обрезки */
//			void update_line_rect(glyphs_line_t &line, bool check_words = true)
//			{
//				CALLTRACE();
//				rectangle_t brect;
//				glyphs_list_t::const_iterator it = line.m_glyphs.begin(), end = line.m_glyphs.end();
//				if (it != end) {
//					brect = (it++)->label_pos;
//					for (; it != end; ++it) {
//						brect.union_with_rect(it->label_pos);
//					}
//				}
//				line.m_bound_rect = brect + line.m_rect.position();
//				if (line.m_glyphs.empty()) {
//					line.m_rect.w = 0.0f;
//				} else {
//					const glyph_data_t &last_glyph = line.m_glyphs.back();
//					if (last_glyph.label_pos.w) {
//						line.m_rect.w = last_glyph.label_pos.x + last_glyph.label_pos.w;
//					} else {
//						line.m_rect.w = last_glyph.label_pos.x + utils::round(last_glyph.advance * m_char_spacing);
//					}
//				}
//				if (check_words) {
//					// Последнее слово могло быть полностью удалено из строки, проверяем
//					words_edges_t::reverse_iterator last_word = line.m_words.rbegin();
//					while (!line.m_words.empty() && last_word->first >= (int)line.size()) {
//						line.m_words.erase(last_word->first);
//					}
//					// Корректируем правую границу последнего слова, если оно есть
//					if (!line.m_words.empty()) {
//						last_word = line.m_words.rbegin();
//						if (last_word->second > (int)line.size() - 1) {
//							last_word->second = (int)line.size() - 1;
//						}
//					}
//				}
//			}
//		};
//		clipper_t clipper(text_dimension, m_char_spacing, font_asc);
//		
//		static const freetype::char_t lf = 10;		///< юникод конца строки
//		static const freetype::char_t spc = 32;		///< юникод пробела
//		static const freetype::char_t elp = 8230;	///< многоточие
//		// Получаем глиф многоточия вместе с его позицией в атласе текстуры.
//		const Font::glyph_texture_data_t ellipsis = m_font->get_glyph(elp, m_load_glyphs);
//		
//		freetype::char_t prev_glyph = 0;
//		size_t word_beg = 0;	///< индекс глифа, начинающего текущее слово
//		size_t word_it = 0;		///< индекс текущего добавляемого в строку глифа
//		size_t char_it = 0;		///< индекс текущего символа
//		bool in_word = false;	///< последний добавленный в строку глиф - часть слова
//		bool proceed = true;	///< признак возможности продолжения обработки текста
//		bool force_cut = false;	///< принудительно порезать слово, если глиф зашел за пределы ширины текстового поля
//		while (char_current != str_end && proceed) {
//		    freetype::char_t unicode = 0;
//			try {
//				unicode = (uint32_t)utf8::next(char_current, str_end);
//			} catch(const std::exception& e) {
//				LOG(ERR) << e.what();
//				break;
//			}
//			
//			// Если текст должен быть расположен в одну строку, то переносы строк игнорируем
//			if (!multiline && unicode == lf) continue;
//			
//			// Флаг, что переносим на новую строку
//			bool new_line = unicode == lf;
//			
//			// Получаем глиф вместе с его позицией в атласе текстуры
//			Font::glyph_texture_data_t glyph_tex_data = m_font->get_glyph(unicode, m_load_glyphs);
//			
//			// Если текст обрезается по одной строке, и глиф валиден, то проверяем, влезает ли он по горизонтали
//			if (!multiline && clip && glyph_tex_data.valid && !utils::StringUtils::is_control(unicode)) {
//				// Если глиф не влезает, ставим символ многоточие и прекращаем заполнение строки
//				if (pen_pos.x + glyph_tex_data.bearing_left + glyph_tex_data.rect.w > text_dimension.w) {
//					// Если не влезает первый глиф текста
//					if (m_lines.empty()) {
//						glyph_tex_data.valid = false;
//						proceed = false;
//					} else {
//						// Если даже многоточие не умещается в ширине поля, то завершаем обработку текста
//						if (!clipper.ellipsize(pen_pos, m_lines.back(), ellipsis)) break;
//						// Добавляем елипсис и выставляем флаг завершения обработки текста
//						glyph_tex_data = ellipsis;
//						unicode = elp;
//						proceed = false;
//						prev_glyph = 0;	// многоточие ставим всегда без кернинга
//					}
//				}
//			}
//			
//			// Если включен перенос по словам, выполняем проверку ширины строки
//			if (multiline && !new_line) {
//				// Если глиф валиден и является видимым
//				if (glyph_tex_data.valid && glyph_tex_data.is_visible()) {
//					// Если не влезает в ширину поля
//					if (pen_pos.x + glyph_tex_data.bearing_left + glyph_tex_data.rect.w > text_dimension.w) {
//						// Если не влезает первый глиф текста
//						if (m_lines.empty()) {
//							glyph_tex_data.valid = false;
//							proceed = false;
//						} else
//						// Если это единственный глиф, который оказался шире текстового поля
//						if (pen_pos.x == 0.0f) {
//							// глиф не может быть отображен вообще
//							glyph_tex_data.valid = false;
//							// прекращаем дальнейшую обработку текста
//							proceed = false;
//						} else {
//							// возводим флаг новой строки
//							new_line = true;
//							// если текущий глиф относится к слову, которое следует перенести на следующую строку
//							if (in_word && !force_cut) {
//								// если это не единственное слово, то переносим текущее слово из текущей строки в новую
//								if (m_lines.back().m_words.size() > 1) {
//									// длина части слова на текущей строке
//									size_t word_left_part = word_it - word_beg + 1;
//									// удаляем начало слова из текущей строки
//									clipper.trim_right(m_lines.back(), word_left_part);
//									// возвращаемся назад по исходному тексту для вставки глифов в новую строку, при этом учитываем,
//									// что текущий неумещающийся глиф сейчас тоже относится к левой части слова (+1 к перемотке назад)
//									for (int i = word_left_part + 1; i > 0; --i) {
//										utf8::prior(char_current, str_beg);
//									}
//									char_it -= word_left_part + 1;
//									// не нужно вставлять текущий глиф, если мы в середине слова; будем вставлять слово заново в новую строку
//									glyph_tex_data.valid = false;
//									// помечаем строку неполной, так как в ней влезла часть слова, которую мы собираемся перенести
//									m_lines.back().m_full = false;
//								} else
//								// это единственное слово, и оно не влезает в строку; нужно переносить посимвольно
//								// если есть строка выше, и в неё может влезть хотя бы что-то, то оставляем часть слова в ней
//								if (m_lines.size() > 1) {
//									glyphs_lines_t::reverse_iterator last_line = m_lines.rbegin();
//									glyphs_line_t &prev_line = *(++last_line);
//									if (!prev_line.m_full) {
//										// длина части слова на текущей строке +количество обрезанных пробелов с конца предыдущей строки
//										size_t word_left_part = prev_line.m_trimmed + word_it - word_beg + 1;
//										// удаляем текущую строку и возвращаемся на предыдущую
//										m_lines.pop_back();
//										pen_pos = m_lines.back().m_last_pen_pos;
//										// возвращаемся назад по исходному тексту для вставки части слова в конец предыдущей строки, при этом учитываем,
//										// что текущий неумещающийся глиф сейчас тоже относится к левой части слова (+1 к перемотке назад)
//										for (int i = word_left_part + 1; i > 0; --i) {
//											utf8::prior(char_current, str_beg);
//										}
//										char_it -= word_left_part + 1;
//										// не нужно вставлять текущий глиф, если мы в середине слова; будем вставлять слово заново в предыдущую строку
//										glyph_tex_data.valid = false;
//										// переключаем флаги так, что слово нужно резать, а не переносить на новую строку
//										force_cut = true;
//										new_line = false;
//									}
//								}
//							}
//							// из текущей строки с конца удаляем все невидимые глифы
//							clipper.trim_right(m_lines.back());
//							// помечаем строку как не последнюю (для justify выравнивания)
//							m_lines.back().m_last = false;
//						}
//					}
//				}
//			}
//			
//			// Обработка флага новой строки - смещаем позицию пера вниз на высоту строки, Х ставим в 0.
//			if (new_line) {
//				pen_pos.x = 0.0f;
//				pen_pos.y += line_height;
//				// Со следующей строки начинается новое слово (даже если это отрезанная часть слова)
//				in_word = false;
//				// Если слово специально разрезали при переносе, значит строка точно заполнена до предела
//				if (force_cut) {
//					glyphs_line_t &line = m_lines.back();
//					line.m_full = true;
//					line.m_trimmed = 0;
//					force_cut = false;
//				}
//				// Если надо перейти на новую линию, то новую строку глифов создаём только если в старой
//				// строке глифов есть элементы. Чтобы не было пустых строк.
//				if (!m_lines.empty() && !m_lines.back().empty()) {
//					m_lines.push_back(glyphs_line_t(rectangle_t(pen_pos, 0.0f, font_height)));
//				}
//			}
//			
//			// Если глиф валиден, является видимым и не является управляющим, то обрабатываем его
//			if (glyph_tex_data.valid && !utils::StringUtils::is_control(unicode)) {
//				// Добавляем новую строку глифов, если строк ещё нет
//				if (m_lines.empty()) {
//					m_lines.push_back(glyphs_line_t(rectangle_t(pen_pos, 0.0f, font_height)));
//				}
//				// Кернинг
//				int kerning = 0;
//				if (prev_glyph) {
//					kerning = m_font->get_offset(prev_glyph, unicode);
//				}
//				// Позиция и размер глифа в тексте
//				rectangle_t glyph_rect(
//					pen_pos.x + glyph_tex_data.bearing_left + kerning,
//					font_asc - glyph_tex_data.bearing_top,
//					glyph_tex_data.rect
//				);
//				// Если включена обрезка и метка многострочная и глиф не помещается в поле по высоте
//				if (clip && multiline && m_lines.back().m_rect.y + glyph_rect.bottom() > text_dimension.h) {
//					// Удаляем текущую строку из списка строк. Если это была единственная строка, заменим её
//					// пустой строкой (чтобы не очищать текущую) и попытаемся вставить в неё многоточие
//					m_lines.pop_back();
//					if (m_lines.empty()) {
//						// Добавляем единственную строку для вставки многоточия. Если требуется не показывать
//						// многоточие, то строку не добавляем, а просто прерываем основной цикл (break)
//						m_lines.push_back(glyphs_line_t(rectangle_t(pen_pos, 0.0f, font_height)));
//					}
//					// Обрезаем текст, выставляя многоточие в конце предыдущей строки
//					glyphs_line_t &line = m_lines.back();
//					pen_pos = line.m_last_pen_pos;
//					// Если даже многоточие не умещается в поле, то завершаем обработку текста
//					if (!clipper.ellipsize(pen_pos, line, ellipsis)) break;
//					// Добавляем елипсис и выставляем флаг завершения обработки текста
//					glyph_tex_data = ellipsis;
//					unicode = elp;
//					proceed = false;
//					// Корректируем параметры глифа
//					kerning = 0;	// многоточие ставим всегда без кернинга
//					glyph_rect = rectangle_t(
//						pen_pos.x + glyph_tex_data.bearing_left,
//						font_asc - glyph_tex_data.bearing_top,
//						glyph_tex_data.rect
//					);
//					// Многоточие должно быть частью слова, если оно 'цепляется' к слову
//					if (!line.m_words.empty()) {
//						words_edges_t::reverse_iterator last_word = line.m_words.rbegin();
//						if (last_word->second >= (int)line.size() - 1) {
//							word_beg = last_word->first;
//							in_word = true;
//						} else {
//							in_word = false;
//						}
//					}
//				}
//				glyphs_line_t &line = m_lines.back();
//				// Корректировка границ строки
//				if (glyph_tex_data.is_visible()) {
//					// В обычном случае берем правую границу текущего глифа
//					line.m_rect.w = glyph_rect.x + glyph_rect.w;
//					// Корректировка очертаний строки - делаем путём объединения, если глиф видим (ширина и высота > 0)
//					if (!line.m_bound_rect) line.m_bound_rect = glyph_rect + position_t(0.0f, pen_pos.y);
//					else line.m_bound_rect.union_with_rect(glyph_rect + position_t(0.0f, pen_pos.y));
//				} else {
//					// Для невидимого глифа нужно прибавить его адванс, иначе в длине строки не будут учтены
//					// невидимые глифы, завершающие её (например, пробелы)
//					const GLfloat advance_real = utils::round(glyph_tex_data.advance * m_char_spacing);
//					line.m_rect.w += advance_real;
//					//Если глиф невидим, аналогичным способом считаем и bounding_rect. Иначе в него не попадёт пробел, стоящий в конце строки.
//					line.m_bound_rect.w += advance_real;
//				}
//				// Создание нового глифа
//				glyph_data_t new_glyph(glyph_rect, glyph_tex_data.rect, glyph_tex_data.advance, kerning, default_color, unicode);
//				// Если заданы цветовые регионы, определяем цвет глифа
//				if (!colors.empty()) {
//					for (color_regions_t::const_reverse_iterator it = colors.rbegin(), end = colors.rend(); it != end; ++it) {
//						if (char_it >= it->first) {
//							new_glyph.color = it->second;
//							break;
//						}
//					}
//				}
//				// Добавление глифа в строку
//				line.push_back(new_glyph);
//				// Смещаем позицию пера в строке для следующего глифа, фиксируем позицию в строке
//				pen_pos.x += utils::round((glyph_tex_data.advance + kerning) * m_char_spacing);
//				line.m_last_pen_pos = pen_pos;
//				// Индекс текущего глифа
//				word_it = line.m_glyphs.size() - 1;
//				// Если глиф не является пробелом, то это часть слова
//				if (unicode != spc) {
//					// если мы не в слове, то фиксируем начало нового слова
//					if (!in_word) {
//						word_beg = word_it;
//						in_word = true;
//					}
//				} else {
//					in_word = false;
//				}
//				// Если мы в слове, то фиксируем конец текущего слова
//				if (in_word) {
//					line.m_words[word_beg] = word_it;
//				}
//			}
//			prev_glyph = unicode;
//			++char_it;
//		}
//	}
//	
//	void FontLayout::process_alignment(const dimension_t &text_dimension, const horizontal_alignment_t h_align, const vertical_alignment_t v_align)
//	{
//		CALLTRACE();
//		// Горизонтальное выравнивание - делается для каждой строки отдельно.
//		for (glyphs_lines_t::iterator it = m_lines.begin(), end = m_lines.end(); it != end; ++it) {
//			glyphs_line_t &line = *it;
//			// Последние строки параграфа не выравниваются по ширине
//			bool can_align = h_align != H_ALIGN_JUSTIFY || !line.m_last;
//			if (can_align) process_h_alignment(text_dimension, h_align, *it);
//		}
//		// Вертикальное выравнивание - делается для всех строк сразу.
//		process_v_alignment(text_dimension, v_align);
//	}
//	
//	void FontLayout::process_h_alignment(const dimension_t &text_dimension, const horizontal_alignment_t h_align, glyphs_line_t &glyphs_line)
//	{
//		CALLTRACE();
//		if (glyphs_line.empty()) return;
//		
//		const rectangle_t bounding_rect = glyphs_line.bounding_rectangle();
//		const position_t bound_diff = glyphs_line.bounding_rectangle().position() - glyphs_line.rectangle().position();
//		GLfloat hor_shift = 0.0f;
//		switch (h_align) {
//			case H_ALIGN_NONE:
//			case H_ALIGN_LEFT:
//				hor_shift = -bound_diff.x;
//				break;
//			case H_ALIGN_MID:
//				hor_shift = utils::round((text_dimension.w - bounding_rect.w) / 2) - bound_diff.x;
//				break;
//			case H_ALIGN_RIGHT:
//				hor_shift = text_dimension.w - bounding_rect.w - bound_diff.x;
//				break;
//			case H_ALIGN_JUSTIFY:
//				const GLfloat line_width = glyphs_line.m_rect.w;
//				const int words_count = glyphs_line.m_words.size();
//				// Для выравнивания H_ALIGN_JUSTIFY используем расстояние между словами
//				hor_shift = floor((text_dimension.w - line_width) / (words_count - 1));
//				break;
//		}
//		
//		if (!hor_shift) return;
//		
//		if (h_align == H_ALIGN_JUSTIFY) {
//			// Если нет слов, то выравнивать нечего
//			if (glyphs_line.m_words.empty()) return;
//			words_edges_t::iterator word_beg = glyphs_line.m_words.begin();
//			words_edges_t::iterator word_end = glyphs_line.m_words.end();
//			// Текущее слово в строке
//			words_edges_t::iterator word_curr = word_beg;
//			// Текущее смещение для глифов
//			GLfloat curr_shift = 0.0f;
//			// Сдвигаем глифы слов
//			for (glyphs_list_t::iterator beg = glyphs_line.m_glyphs.begin(), it = beg, end = glyphs_line.m_glyphs.end(); it != end; ++it) {
//				glyph_data_t &glyph_data = *it;
//				// Первое слово не двигаем
//				if (word_curr != word_beg) {
//					// Сдвигаем глиф на смещение для текущего слова
//					glyph_data.label_pos.x += curr_shift;
//				}
//				// Если текущий глиф завершает текущее слово, то
//				if (word_curr != word_end && it - beg == word_curr->second) {
//					// увеличиваем сдвиг для следующего слова
//					curr_shift += hor_shift;
//					// берем следующее слово строки
//					++word_curr;
//				}
//			}
//			GLfloat diff = text_dimension.w - glyphs_line.m_rect.w;
//			glyphs_line.m_rect.w += diff;
//			glyphs_line.m_bound_rect.w += diff;
//		} else {
//			glyphs_line.m_rect.x += hor_shift;
//			glyphs_line.m_bound_rect.x += hor_shift;
//		}
//	}
//	
//	void FontLayout::process_v_alignment(const dimension_t &text_dimension, const vertical_alignment_t v_align)
//	{
//		CALLTRACE();
//		if (m_lines.empty()) return;
//		//Это рект не bounding! Который используется для выравнивания!!!
//		const rectangle_t rect = rectangle();
//		
//		GLfloat ver_shift = 0.0f;
//		switch (v_align) {
//			case V_ALIGN_NONE:
//			case V_ALIGN_TOP:
//				ver_shift = 0.0f;
//				break;
//			case V_ALIGN_MID:
//			{
//				ver_shift = utils::round((text_dimension.h - rect.h) / 2);
//				// Корректируем высоту по meanline (расположена выше baseline на x-height/2)
//				const GLfloat x_height = xheight();
//				if (m_xheight_inited) {
//					// На сколько meanline ниже середины строки
//					const GLfloat delta = m_font->ascender() - (x_height + m_font->font_height()) / 2;
//					// Корректируем смещение
//					ver_shift -= floor(delta);
//				}
//				break;
//			}
//			case V_ALIGN_BOTTOM:
//				ver_shift = text_dimension.h - rect.h;
//				break;
//		}
//		
//		if (!ver_shift) return;
//		
//		//Смещаем все строки на ver_shift.
//		for (glyphs_lines_t::iterator beg = m_lines.begin(), it = beg, end = m_lines.end(); it != end; ++it) {
//			glyphs_line_t &glyph_line = *it;
//			glyph_line.m_rect.y += ver_shift;
//			glyph_line.m_bound_rect.y += ver_shift;
//		}
//	}
//	
//	rectangle_t FontLayout::measure_text(const std::string& text)
//	{
//		CALLTRACE();
//		process(dimension_t(), text, false, false);
//		return bounding_rectangle();
//	}
//	
//	rectangle_t FontLayout::rectangle()
//	{
//		CALLTRACE();
//		glyphs_lines_t::const_iterator it = m_lines.begin(), end = m_lines.end();
//		if (it == end) return rectangle_t();
//		const glyphs_line_t &line = *(it++);
//		rectangle_t rect = line.m_rect;
//		for (; it != end; ++it) {
//			const glyphs_line_t &next_line = *it;
//			const rectangle_t &line_rect = next_line.m_rect;
//			if (!line_rect.dimension()) continue;
//			rect.union_with_rect(line_rect);
//		}
//		return rect;
//	}
//	
//	rectangle_t FontLayout::bounding_rectangle()
//	{
//		CALLTRACE();
//		glyphs_lines_t::const_iterator it = m_lines.begin(), end = m_lines.end();
//		if (it == end) return rectangle_t();
//		const glyphs_line_t &line = *(it++);
//		rectangle_t bound_rect = line.m_bound_rect;
//		for (; it != end; ++it) {
//			const glyphs_line_t &next_line = *it;
//			const rectangle_t &line_brect = next_line.m_bound_rect;
//			if (!line_brect.dimension()) continue;
//			bound_rect.union_with_rect(line_brect);
//		}
//		return bound_rect;
//	}
//	
//	GLfloat FontLayout::xheight()
//	{
//		CALLTRACE();
//		// Текущий шрифт не изменяется для текущего экземпляра FontLayout, поэтому значение x-height 
//		// можем вычислить один раз и использовать это значение сколько угодно раз
//		if (m_xheight_inited) return m_xheight;
//		// Если x-height еще не определен для данного шрифта, определяем
//		m_xheight = 0.0f;
//		static const std::string x = "x";
//		freetype::char_t unicode = 0;
//		try {
//			std::string::const_iterator beg = x.begin();
//			const std::string::const_iterator end = x.end();
//			unicode = (uint32_t)utf8::next(beg, end);
//			const Font::glyph_texture_data_t glyph = m_font->get_glyph(unicode, false);
//			m_xheight_inited = glyph.valid;
//			if (glyph.valid) m_xheight = glyph.rect.h;
//		} catch(const std::exception& e) {
//			LOG(ERR) << e.what();
//		}
//		return m_xheight;
//	}
//	
//}
