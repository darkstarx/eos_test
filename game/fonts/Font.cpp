#include <fonts/Font.hpp>
#include <freetype/FTFace.hpp>
#include <freetype/FTGlyph.hpp>
#include <resources/Image.hpp>
#include <graphics/Context.hpp>
#include <graphics/Texture.hpp>
#include <graphics/TextureManager.hpp>
#include <utils/log.hpp>
#include <utils/bytearray.hpp>


namespace fonts
{
	
	Font::Font(const freetype::FTFaceSPtr &face)
	: m_face(face)
	, m_size(face->size())
	{
		ASSERT(m_face) << "Шрифт выгружен";
	}
	
	
	Font::~Font()
	{
	}
	
	
	int Font::font_height() const
	{
		ASSERT(m_face) << "Шрифт выгружен";
		if (!m_face) return 0;
		return m_face->font_height();
	}
	
	
	int Font::ascender() const
	{
		ASSERT(m_face) << "Шрифт выгружен";
		if (!m_face) return 0;
		return m_face->ascender();
	}
	
	
	int Font::descender() const
	{
		ASSERT(m_face) << "Шрифт выгружен";
		if (!m_face) return 0;
		return m_face->descender();
	}
	
	
	size_t Font::texture_size() const
	{
		return m_glyphs.atlas ? m_glyphs.atlas->size() : 0;
	}
	
	
	bool Font::is_loaded() const
	{
		return m_glyphs.atlas && m_glyphs.atlas->initialized();
	}
	
	
	void Font::unload()
	{
		ASSERT(m_glyphs.atlas->initialized());
		unload_font_data();
		LOG(INFO) << this << " Шрифт выгружен: " << m_glyphs.atlas->description();
	}
	
	
	void Font::reload()
	{
		ASSERT(!m_glyphs.atlas->initialized());
		load_font_data();
		LOG(INFO) << this << " Шрифт перезагружен: " << m_glyphs.atlas->description();
	}
	
	
	glyph_t Font::get_glyph(const freetype::char_t unicode, bool need_bitmap)
	{
		glyph_t result;
		
		if (!m_face || !m_glyphs.atlas) return result;
		
		// Если глиф есть в кэше, возвращаем его
		glyphs_t::const_iterator it = m_glyphs.map.find(unicode);
		if (it != m_glyphs.map.end()) {
			result = it->second;
			// Если битмап в глифе есть и уже загружен либо пока не может быть загружен в атлас
			// или он не нужен, возвращаем глиф из кэша
			if ((result.has_bitmap && (result.loaded || !m_glyphs.atlas->initialized())) || !need_bitmap) return result;
		}
		
		// Получаем глиф из шрифта, если он еще не получен, или еще не загружен требуемый битмап
		const freetype::FTGlyphSPtr glyph = m_face->get_glyph(unicode, need_bitmap);
		
		// Если пустой глиф, возвращаем пустой результат
		if (!glyph) {
			result.valid = false;
			return result;
		}
		
		// Заполняем атрибуты результата
		result.rect = graphics::rectangle_t(0.0f, 0.0f, glyph->width(), glyph->height());
		result.advance = glyph->advance();
		result.bearing_left = glyph->bearing_left();
		result.bearing_top = glyph->bearing_top();
		
		// Если битмап не нужен либо это управляющий символ, то битмап в атлас не попадает
		if (!need_bitmap || glyph->is_control()) {
			// кэшируем глиф без битмапа
			m_glyphs.map[unicode] = result;
			// возвращаем глиф без битмапа
			return result;
		}
		
		// Получаем битмап глифа
		const resources::ImageSPtr bitmap = glyph->get_biamap();
		
		// Если битмап пустой, возвращаем пустой результат
		if (!bitmap) {
			// кэшируем глиф без битмапа, всё равно битмап не появится
			m_glyphs.map[unicode] = result;
			// возвращаем глиф без битмапа
			return result;
		}
		
		// Зазор между глифами 1 пиксел
		static const GLfloat glyphs_spacing = 1.0f;
		
		// Позиция глифа в текстуре
		graphics::rectangle_t glyph_tex_rect(m_glyphs.next_glyph_pos, bitmap->width(), bitmap->height());
		
		if (glyph_tex_rect.y + glyph_tex_rect.h > m_glyphs.atlas->dimension().h) {
			// Значит глиф не влезает по вертикали, переносить на новую строку уже нет смысла.
			LOG(WARN) << "Невозможно отобразить символ " << unicode << ": атлас переполнен!";
			// Возвращаем пустой результат.
			return result;
		}
		
		// Если правый край глифа выходит за пределы текстуры, переносим его на новую строку.
		if (glyph_tex_rect.x + glyph_tex_rect.w > m_glyphs.atlas->dimension().w) {
			glyph_tex_rect.x = 0.0f;
			glyph_tex_rect.y += m_glyphs.curr_line_height + glyphs_spacing;
			
			if (glyph_tex_rect.y + glyph_tex_rect.h > m_glyphs.atlas->dimension().h) {
				// Значит перенесённый на новую строку глиф уже не влезает по вертикали.
				LOG(WARN) << "Невозможно отобразить символ " << unicode << ": атлас переполнен!";
				// Возвращаем пустой результат.
				return result;
			}
			// Позицию следующего глифа ставим на новую строку только когда известно, что глиф влез по вертикали.
			m_glyphs.next_glyph_pos = glyph_tex_rect.position();
			// Обнуляем максимальную высоту в строке
			m_glyphs.curr_line_height = 0;
		}
		
		// Смещаем координату следующего глифа вправо, учитываем зазор.
		m_glyphs.next_glyph_pos.x += glyph_tex_rect.w + glyphs_spacing;
		
		// Считаем максимальную высоту глифа в строке только когда известно, что глиф влез.
		if (glyph_tex_rect.h > m_glyphs.curr_line_height) m_glyphs.curr_line_height = glyph_tex_rect.h;
		
		// Помечаем, что битмап глифа должен быть в атласе (или при восстановлении текстуры, должен появиться в атласе)
		result.has_bitmap = true;
		
		// Позиция глифа посчитана. Теперь необходимо добавить битмап глифа в текстуру.
		// Делаем это, только если текстура не выгружена.
		if (m_glyphs.atlas->initialized()) {
			copy_bitmap_to_texture(m_glyphs.atlas, glyph_tex_rect, bitmap->data());
			result.loaded = true;
		}
		
		// Заполняем регион битмапа в атласе, кладём в кэш и возвращаем его.
		result.rect = glyph_tex_rect;
		m_glyphs.map[unicode] = result;
		
		return result;
	}
	
	
	int Font::get_offset(const freetype::char_t char1, const freetype::char_t char2)
	{
		ASSERT(m_face) << "Шрифт выгружен";
		if (!m_face) return 0;
		return m_face->get_offset(char1, char2);
	}
	
	
	unsigned int Font::atlas_dimension()
	{
		if (m_size < 8) return 128;
		else if (m_size < 24) return 256;
		else if (m_size < 56) return 512;
		else return 1024;
	}
	
	
	void Font::load_font_data()
	{
		ASSERT(m_face) << "Шрифт выгружен";
		
		// Новую текстуру создаём только если она выгружалась
		if (!m_glyphs.atlas || !m_glyphs.atlas->initialized()) {
			if (!m_glyphs.atlas) {
				unsigned int tex_dimension = atlas_dimension();
				m_glyphs.atlas = texmgr().create_empty_texture(tex_dimension,  tex_dimension, 1);
				m_glyphs.atlas->set_description(m_face->description());
			} else if (!m_glyphs.atlas->initialized()) {
				m_glyphs.atlas->reload(utils::any_f(), this);
			}

			//Пробегаемся по всему кэшу глифов и загружаем их битмапы в текстуру.
			for (glyph_texture_map_t::iterator it=m_glyph_map.begin(), end=m_glyph_map.end(); it!=end; ++it) {
				glyph_texture_data_t &glyph_data = it->second;
				if (glyph_data.has_bitmap && !glyph_data.loaded) {
					const FTGlyphSPtr glyph = m_face->get_glyph(it->first, true);
					if (glyph) {
						load_bitmap_to_texture(glyph_data.rect, glyph->get_biamap()->data());
						glyph_data.loaded = true;
					}
				}
			}
		}
	}
	
	
	void Font::unload_font_data()
	{

	}
	
	
	void Font::copy_bitmap_to_texture(const graphics::TextureSPtr &texture, const graphics::rectangle_t &rect, const utils::bytearray &data)
	{
		ASSERT(texture->initialized());
		graphics::gl::ctx()->pixel_store(GL_UNPACK_ALIGNMENT, 1);
		texture->use();
		graphics::gl::ctx()->tex_subimage(rect.x, rect.y, rect.w, rect.h, GL_ALPHA, GL_UNSIGNED_BYTE, data);
	}
	
}
