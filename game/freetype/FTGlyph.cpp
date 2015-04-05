#include <freetype/FTGlyph.hpp>
#include <freetype/FTFace.hpp>
#include <freetype/macro.hpp>
#include <resources/Image.hpp>
#include <utils/log.hpp>


namespace freetype
{
	
	FTGlyph::FTGlyph(const FT_Face &face, const char_t unicode, bool need_bitmap)
	: m_unicode(unicode)
	, m_glyph(NULL)
	, m_advance(0)
	, m_width(0)
	, m_height(0)
	, m_bearing_left(0)
	, m_bearing_top(0)
	{
		// Для управляющих символов ничего не загружаем
		if (is_control()) return;
		
		// По умолчанию не загружаем битмап сразу
		const FT_Int load_flags = FT_LOAD_NO_BITMAP;
		
		// Загружаем глиф в шрифт
		FTCHECK( FT_Load_Char(face, m_unicode, load_flags) );
		
		// Получаем глиф
		FTCHECK( FT_Get_Glyph(face->glyph, &m_glyph) );
		
		// Заполняем advance, переводим в пиксели (здесь advance в формате 16.16)
		m_advance = m_glyph->advance.x >> 16;
		
		// Получаем метрики глифа. Если загружен битмап, смещения будут перезаписаны из структуры FT_BitmapGlyph.
		m_width = FT_PIXEL_26_6(face->glyph->metrics.width);
		m_height = FT_PIXEL_26_6(face->glyph->metrics.height);
		m_bearing_left = FT_PIXEL_26_6(face->glyph->metrics.horiBearingX);
		m_bearing_top = FT_PIXEL_26_6(face->glyph->metrics.horiBearingY);
		
		// Если битмап не нужен, то выходим без его загрузки
		if (!need_bitmap) return;
		
		// Преобразуем глиф в bitmap
		FTCHECK( FT_Glyph_To_Bitmap( &m_glyph, FT_RENDER_MODE_NORMAL, 0, 1 ) );
		const FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)m_glyph;
		
		// Запоминаем смещения из битмапа
		m_bearing_left = bitmap_glyph->left;
		m_bearing_top = bitmap_glyph->top;
	}
	
	
	FTGlyph::~FTGlyph()
	{
		// Освобождаем глиф
		FT_Done_Glyph(m_glyph);
	}
	
	
	resources::ImageSPtr FTGlyph::get_biamap()
	{
		// Если мы в конструктор передали need_bitmap = false, то возвращаем пустой битмап
		if (m_glyph->format != FT_GLYPH_FORMAT_BITMAP) return resources::ImageSPtr();
		const FT_Bitmap &bitmap = ((FT_BitmapGlyph)m_glyph)->bitmap;
		
		// Создаём и возвращаем bitmap, глубиной цвета 1 байт
		return resources::ImageSPtr(new resources::Image(
			utils::bytearray(bitmap.buffer, bitmap.width * bitmap.rows),
			bitmap.width,
			bitmap.rows,
			1
		));
	}
	
}
