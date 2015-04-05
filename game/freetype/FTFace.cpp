#include <freetype/FTFace.hpp>
#include <freetype/FTLibrary.hpp>
#include <freetype/FTGlyph.hpp>
#include <freetype/macro.hpp>
#include <utils/log.hpp>
#include <utils/strbuilder.hpp>
#include <cmath>


namespace freetype
{
	
	FTFace::FTFace(const utils::bytearray_sptr_t &font_data, const unsigned int font_size)
	: m_font_data(font_data)
	, m_library(FTLibrary::instance())
	, m_font_size(font_size)
	, m_font_height(0)
	, m_face(NULL)
	{
		// Загружаем шрифт
		FTLOG( FT_New_Memory_Face(m_library->get(), font_data->get(), font_data->size(), 0, &m_face) );
		if (!m_face) return;
		
		// Устанавливаем размеры 1/64 пиксела
		FTCHECK( FT_Set_Char_Size(m_face, font_size << 6, font_size << 6, 72, 72) );
		const float height_metric = FT_PIXEL_26_6(m_face->size->metrics.height);
		m_font_height = std::floor(height_metric);
	}
	
	
	FTFace::~FTFace()
	{
		if (m_face) {
			FT_Done_Face(m_face);
		}
	}
	
	
	FTGlyphSPtr FTFace::get_glyph(const char_t unicode, const bool need_bitmap) const
	{
		if (!m_face) {
			LOG(ERR) << "m_face is NULL";
			return FTGlyphSPtr();
		}
		return FTGlyphSPtr(new FTGlyph(m_face, unicode, need_bitmap));
	}
	
	
	std::string FTFace::description() const
	{
		if (!m_face) {
			LOG(ERR) << "m_face is NULL";
			return std::string();
		}
		return utils::strbuilder() << m_face->family_name << " " << m_face->style_name << ", " << m_font_size;
	}
	
	
	int FTFace::get_offset(const char_t char1, const char_t char2)
	{
		if (!m_face) {
			LOG(ERR) << "m_face is NULL";
			return 0;
		}
		if (!FT_HAS_KERNING(m_face)) return 0;
		const FT_UInt glyph_index1 = FT_Get_Char_Index(m_face, char1);
		const FT_UInt glyph_index2 = FT_Get_Char_Index(m_face, char2);
		FT_Vector delta;
		FT_Get_Kerning(m_face, glyph_index1, glyph_index2, FT_KERNING_DEFAULT, &delta);
		return FT_PIXEL_26_6(delta.x);
	}
	
	
	FT_Pos FTFace::ascender() const
	{
		if (!m_face) {
			LOG(ERR) << "m_face is NULL";
			return 0;
		}
		return FT_PIXEL_26_6(m_face->size->metrics.ascender);
	}
	
	
	FT_Pos FTFace::descender() const
	{
		if (!m_face) {
			LOG(ERR) << "m_face is NULL";
			return 0;
		}
		return FT_PIXEL_26_6(m_face->size->metrics.descender);
	}
	
}
