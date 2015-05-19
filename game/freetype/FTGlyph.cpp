#include <freetype/FTGlyph.hpp>
#include <freetype/FTFace.hpp>
#include <freetype/FTLibrary.hpp>
#include <freetype/macro.hpp>
#include <resources/Image.hpp>
#include <utils/log.hpp>
#include <ftstroke.h>
#include <deque>
#include <algorithm>


namespace freetype
{
	
	/** \brief Структура, описывающая набор пикселей в строке битмапа
	 * Хранит позицию в битмапе, длину (кол-во подряд идущих пикселей в строке) и их цвет.
	 */
	struct span_t {
		int x, y;
		int len;
		int color;
	};
	typedef std::deque<span_t> spans_t;
	
	
	/** \brief Прямоугольник с целочисленными скалярами
	 */
	struct rectangle_t {
		rectangle_t(int l, int r, int t, int b) : l(l), t(t), r(r), b(b) {}
		void include(const span_t &span)
		{
			l = std::min(l, span.x);
			t = std::min(t, span.y);
			r = std::max(r, span.x + span.len);
			b = std::max(b, span.y);
		}
		int width() { return r - l + 1; }
		int height() { return b - t + 1; }
		int l, t, r, b;
	};
	
	
	/** \brief Обработать пиксели текущей строки битмапа
	 */
	void process_spans(int y, int count, const FT_Span *spans, void *user)
	{
		spans_t *all_spans = static_cast<spans_t*>(user);
		std::size_t idx = all_spans->size();
		all_spans->resize(idx + count);
		for (int i = 0; i < count; ++i, ++idx) {
			span_t &span = all_spans->at(idx);
			span.x = spans[i].x;
			span.y = y;
			span.len = spans[i].len;
			span.color = spans[i].coverage;
		}
	}
	
	
	/** \class FTGlyph */
	
	FTGlyph::FTGlyph(const FT_Face &face, const char_t unicode, bool need_bitmap, glyph_outline_t outline_type, float outline_width)
		: m_unicode(unicode)
		, m_library(FTLibrary::instance())
		, m_advance(0)
		, m_width(0)
		, m_height(0)
		, m_bearing_left(0)
		, m_bearing_top(0)
	{
		// Для управляющих символов ничего не загружаем
		if (is_control()) return;
		// По умолчанию не загружаем битмап сразу
		const FT_Int load_flags = FT_LOAD_NO_BITMAP | FT_LOAD_NO_HINTING;
		// Получаем индекс нужного глифа
		FT_UInt gindex = FT_Get_Char_Index(face, unicode);
		// Загружаем нужный глиф
		FTCHECK(FT_Load_Glyph(face, gindex, load_flags))
		// Получаем глиф
		FT_Glyph glyph;
		FTCHECK(FT_Get_Glyph(face->glyph, &glyph))
		// Заполняем advance, переводим в пиксели. Здесь advance в формате 16.16
		m_advance = glyph->advance.x >> 16;
		// Получаем начальные метрики глифа
		m_width = FT_PIXEL_26_6(face->glyph->metrics.width);
		m_height = FT_PIXEL_26_6(face->glyph->metrics.height);
		m_bearing_left = FT_PIXEL_26_6(face->glyph->metrics.horiBearingX);
		m_bearing_top = FT_PIXEL_26_6(face->glyph->metrics.horiBearingY);
		// Нам нужна поддержка контуров для формирования битмапа
		if (face->glyph->format != FT_GLYPH_FORMAT_OUTLINE || !need_bitmap) {
			// Освобождаем глиф и завершаем конструирование
			FT_Done_Glyph(glyph);
			return;
		}
		// Готовим набор спанов (не забываем, что ось Y в шрифтах перевернута)
		spans_t all_spans;
		// Параметры для растеризации векторов в спаны
		FT_Raster_Params params;
		params.target = &face->glyph->bitmap;
		params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
		params.gray_spans = process_spans;
		params.user = &all_spans;
		// Очертания глифа
		FT_Outline *outline = &face->glyph->outline;
		// Если требуется обводка глифа
		if (outline_type != glyph_outline_t::none) {
			// Создаем строкер для получения обводки
			FT_Stroker stroker;
			FTCHECK(FT_Stroker_New(m_library->get(), &stroker))
				FT_Stroker_Set(stroker, outline_width * 64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
			// Выполняем обводку глифа
			switch (outline_type) {
			case glyph_outline_t::in:
				FTCHECK(FT_Glyph_StrokeBorder(&glyph, stroker, 1, 1))
					break;
			case glyph_outline_t::out:
				FTCHECK(FT_Glyph_Stroke(&glyph, stroker, 1))
					break;
			case glyph_outline_t::full:
				FTCHECK(FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1))
					break;
			default:
				break;
			}
			// Получаем очертания обводки
			outline = &reinterpret_cast<FT_OutlineGlyph>(glyph)->outline;
			// Освобождаем строкер, он нам больше не потребуется
			FT_Stroker_Done(stroker);
		}
		// Выполняем растеризацию аутлайна в спаны изображения
		FT_Outline_Render(m_library->get(), outline, &params);
		// Освобождаем глиф вместе с очертаниями
		FT_Done_Glyph(glyph);
		// Если спанов нет, значит изображение отсутствует
		if (all_spans.empty()) return;
		// Вычисляем границы изображения глифа
		const span_t &first_span = all_spans.front();
		rectangle_t rect(first_span.x, first_span.y, first_span.x + first_span.len, first_span.y);
		for (spans_t::iterator it = all_spans.begin(), end = all_spans.end(); it != end; ++it) {
			span_t &span = *it;
			rect.include(span);
		}
		// Определяем метрики глифа по границам изображения
		m_width = rect.width();
		m_height = rect.height();
		m_bearing_left = rect.l;	// сдвиг глифа по горизонтали от пера
		m_bearing_top = rect.b + 1;	// сдвиг глифа по вертикали от верхней границы строки (не забываем про перевернутую ось Y)
		// Создаем битмап по размерам глифа
		m_bitmap.reset(new resources::Image(
			utils::bytearray(m_width * m_height),
			m_width, m_height, 8
		));
		// Заполняем битмап нулями
		memset(m_bitmap->data().get(), 0, m_bitmap->data().size());
		// Заполняем битмап пикселями из спанов
		for (spans_t::iterator it = all_spans.begin(), end = all_spans.end(); it != end; ++it) {
			span_t &span = *it;
			unsigned char *ptr = m_bitmap->data().get();
			// Не забываем перевернуть ось Y
			ptr += (m_height - 1 - span.y + rect.t) * m_width + span.x - rect.l;
			memset(ptr, span.color, span.len);
		}
	}
	
	
	FTGlyph::~FTGlyph()
	{
	}
	
	
	resources::ImageSPtr FTGlyph::get_bitmap()
	{
		return m_bitmap;
	}
	
}
