#include "Font.hpp"
#include "checkerror.hpp"
#include <resources/Factory.hpp>
#include <graphics/Texture2D.hpp>
#include <utils/memory_info.hpp>
#include <utils/log.hpp>
#include <utils/unused.hpp>
#include <utils/numbers.hpp>
#include <utils/toString.hpp>
#include <freetype/FTFace.hpp>
#include <freetype/FTGlyph.hpp>
#include <stdio.h>
#include <math.h>


namespace graphics
{

	Font::Font(resources::Factory *factory, const FTFaceSPtr &face, const reload_font_f &reload_face,
		const std::string &source, freetype::glyph_outline_t outline, float outline_width
	)
	: m_outline(outline)
	, m_outline_width(outline_width)
	, m_source(source)
	, m_factory(factory)
	, m_size(face->size())
	, m_reload_face(reload_face)
	, m_use_count(0)
	, m_face(face)
	, m_max_height_in_line(0.0f)
	{
		load_font_data();
	}

	Font::~Font()
	{
		ASSERT(!m_use_count) << ": " << VAR_DUMP(m_use_count);
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
		return m_texture ? m_texture->texture_size() : 0;
	}

	void Font::lock_unload()
	{
		++m_use_count;
	}

	void Font::unlock_unload()
	{
		if (m_use_count > 0) {
			--m_use_count;
		}
	}

	bool Font::unloading_unlocked() const
	{
		return !m_use_count;
	}

	bool Font::is_unloaded() const
	{
		return m_texture->reload_data().is_unloaded;
	}

	void Font::unload()
	{
		ASSERT(unloading_unlocked());
		ASSERT(!is_unloaded());
		unload_font_data();
		LOG(INFO) << this << " Шрифт выгружен: " << m_texture->description();
	}

	void Font::reload()
	{
		ASSERT(is_unloaded());
		load_font_data();
		LOG(INFO) << this << " Шрифт перезагружен: " << m_texture->description();
	}

	Font::glyph_t Font::get_glyph(const freetype::char_t unicode, bool need_bitmap)
	{
		ASSERT(m_texture);
		
		glyph_t result;
		
		ASSERT(m_face) << "Шрифт выгружен";
		if (!m_face) return result;
		
		// Если глиф есть в кэше, возвращаем его
		glyphs_t::const_iterator it = m_glyphs.find(unicode);
		if (it != m_glyphs.end()) {
			result = it->second;
			// Если битмап в глифе есть и уже загружен либо пока не может быть загружен в атлас
			// или он не нужен, возвращаем глиф из кэша
			if ((result.has_bitmap && (result.loaded || is_unloaded())) || !need_bitmap) return result;
		}
		
		// Получаем глиф из шрифта, если он еще не получен, или еще не загружен требуемый битмап
		const graphics::FTGlyphSPtr glyph = m_face->get_glyph(
			unicode, need_bitmap, m_outline, m_outline_width
		);
		
		// Если пустой глиф, возвращаем пустой результат
		if (!glyph) {
			result.valid = false;
			return result;
		}
		
		// Заполняем атрибуты результата
		result.rect = rectangle_t(0.0f, 0.0f, glyph->width(), glyph->height());
		result.advance = glyph->advance();
		result.bearing_left = glyph->bearing_left();
		result.bearing_top = glyph->bearing_top();
		
		// Если битмап не нужен либо это управляющий символ, то битмап в атлас не попадает
		if (!need_bitmap || glyph->is_control()) {
			// кэшируем глиф без битмапа
			m_glyphs[unicode] = result;
			// возвращаем глиф без битмапа
			return result;
		}
		
		// Получаем битмап глифа
		const BitmapSPtr bitmap = glyph->get_biamap();
		
		// Если битмап пустой, возвращаем пустой результат
		if (!bitmap) {
			// кэшируем глиф без битмапа, всё равно битмап не появится
			m_glyphs[unicode] = result;
			// возвращаем глиф без битмапа
			return result;
		}
		
		// Зазор между глифами 1 пиксел
		static const GLfloat glyphs_spacing = 1.0f;
		
		// Позиция глифа в текстуре
		rectangle_t glyph_texture_rect(m_next_glyph_pos, bitmap->width(), bitmap->height());
		
		if (glyph_texture_rect.y + glyph_texture_rect.h > m_texture->height()) {
			// Значит глиф не влезает по вертикали, переносить на новую строку уже нет смысла.
			LOG(WARN) << "Невозможно отобразить символ " << unicode << ": атлас переполнен!";
			// Возвращаем пустой результат.
			return result;
		}
		
		// Если правый край глифа выходит за пределы текстуры, переносим его на новую строку.
		if (glyph_texture_rect.x + glyph_texture_rect.w > m_texture->width()) {
			glyph_texture_rect.x = 0;
			glyph_texture_rect.y += m_max_height_in_line + glyphs_spacing; //Учитываем зазор по вертикали.
			
			if (glyph_texture_rect.y + glyph_texture_rect.h > m_texture->height()) {
				// Значит перенесённый на новую строку глиф уже не влезает по вертикали.
				LOG(WARN) << "Невозможно отобразить символ " << unicode << ": атлас переполнен!";
				// Возвращаем пустой результат.
				return result;
			}
			
			// Позицию следующего глифа ставим на новую строку только когда известно, что глиф влез по вертикали.
			m_next_glyph_pos = glyph_texture_rect.position();
			
			// Обнуляем максимальную высоту в строке
			m_max_height_in_line = 0;
		}
		
		// На данный момент уже известно, что глиф успешно влезает в текстуру.
		
		// Смещаем координату следующего глифа вправо, учитываем зазор.
		m_next_glyph_pos.x += glyph_texture_rect.w + glyphs_spacing;
		
		// Считаем максимальную высоту глифа в строке только когда известно, что глиф влез.
		if (glyph_texture_rect.h > m_max_height_in_line) m_max_height_in_line = glyph_texture_rect.h;
		
		// Помечаем, что битмап глифа должен быть в атласе (или при восстановлении текстуры, должен появиться в атласе)
		result.has_bitmap = true;
		
		// Позиция глифа посчитана. Теперь необходимо добавить битмап глифа в текстуру.
		// Делаем это, только если текстура не выгружена.
		if (!is_unloaded()) {
			const bool fill_ok = load_bitmap_to_texture(m_texture, glyph_texture_rect, bitmap->data());
			//Если не удалось заполнить, возвращаем глиф без битмапа, не кэшируем
			if (!fill_ok) return result;
			result.loaded = true;
		}
		
		// Заполняем регион битмапа в атласе, кладём в кэш и возвращаем его.
		result.rect = glyph_texture_rect;
		m_glyphs[unicode] = result;
		return result;
	}

	int Font::get_offset(const freetype::char_t char1, const freetype::char_t char2)
	{
		ASSERT(m_face) << "Шрифт выгружен";
		if (!m_face) return 0;
		return m_face->get_offset(char1, char2);
	}

	void Font::load_font_data()
	{
		// Блокируем выгрузку шрифта. Он может выгрузиться, если не хватит памяти для представления или атласа.
		lock_unload();
		// Новый шрифт создаём только если он выгружался
		if (!m_face) {
			// Создаём объект шрифта freetype по колбеку
			m_face = m_reload_face();
			DLOG(INFO) << this << ": Данные шрифта загружены: " << m_face->description();
		}
		// Новую текстуру создаём только если она выгружалась
		if (!m_texture || m_texture->reload_data().is_unloaded) {
			if (!m_texture) {
				// Создаем пустой атлас
				m_texture = make_empty_font_texture(m_factory, atlas_size());
				
				m_texture->set_description(
					m_face->description() +
					(m_outline == freetype::glyph_outline_t::none || utils::is_almost_equal(m_outline_width, 0.0f)
						? ""
						: ", " + utils::to_string(m_outline) + " " + utils::to_string(m_outline_width)
					)
				);
			} else if (m_texture->reload_data().is_unloaded) {
				m_texture->reload(utils::any_f(), this);
			}
			// Пробегаемся по всему кэшу глифов и загружаем их битмапы в текстуру
			for (auto &it : m_glyphs) {
				glyph_t &glyph_data = it.second;
				if (glyph_data.has_bitmap && !glyph_data.loaded) {
					const FTGlyphSPtr glyph = m_face->get_glyph(it.first, true, m_outline, m_outline_width);
					if (glyph) {
						if (load_bitmap_to_texture(m_texture, glyph_data.rect, glyph->get_biamap()->data())) {
							glyph_data.loaded = true;
						} else {
							LOG(ERR) << "Не удалось загрузить глиф для атласа " << m_texture->description();
						}
					}
				}
			}
		}
		// Атлас и представление шрифта созданы, можно снаять блокировку выгрузки шрифта
		unlock_unload();
	}

	void Font::unload_font_data()
	{
		// Помечаем глифы как не загруженные
		for (auto &it : m_glyphs) {
			glyph_t &glyph_data = it.second;
			glyph_data.loaded = false;
		}
		// Текстуру разрушаем обязательно, по ней определяется, выгружен ли шрифт
		m_texture->unload();
		// Выгружаем непосредственно ttf-шрифт
		m_face.reset();
	}

	inline int Font::atlas_size()
	{
		// Вместо предсказателя используем эмпирический хак - указываем стандартное разрешение текстур
		// в зависимости от размера шрифта
		if (m_size < 8) return 128;
		else if (m_size < 24) return 256;
		else if (m_size < 56) return 512;
		else return 1024;
	}

	bool Font::load_bitmap_to_texture(const graphics::TextureSPtr &texture, const rectangle_t &texture_rect, const utils::bytearray &data)
	{
		ASSERT(texture->is_valid());
		GLELOG( glPixelStorei(GL_UNPACK_ALIGNMENT, 1) );
		texture->glBindTexture();
		return texture->glTexSubImage2D(
			texture_rect.x, texture_rect.y, texture_rect.w, texture_rect.h,
			GL_ALPHA, GL_UNSIGNED_BYTE, data
		);
	}

	Texture2DSPtr Font::make_empty_font_texture(const resources::Factory *factory, int size)
	{
		const utils::bytearray bitmap_data(size * size);
		memset(bitmap_data.get(), 0, bitmap_data.size());

		//Создаём пустой bitmap для текстуры, глубина пикселя 1 байт
		const bitmap bitmap(bitmap_data, size, size, 8);

		const Texture2DSPtr result = factory->make_texture2d(bitmap, false, false);
		RELOAD_TEXTURE_FOREGROUND(result, bind(&Font::make_empty_font_texture, factory, size));
		return result;
	}
}
