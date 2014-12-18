#include <graphics/Texture.hpp>
#include <graphics/TextureManager.hpp>
#include <graphics/Context.hpp>
#include <resources/Image.hpp>
#include <utils/log.hpp>


namespace graphics
{
	
	Texture::Texture(GLenum unit)
	: m_initialized(false)
	, m_id(0)
	, m_unit(unit)
	{
	}
	
	
	Texture::~Texture()
	{
		if (m_initialized) deinitialize();
		texmgr().release(this);
	}
	
	
	void Texture::initialize(const resources::ImageSPtr &image)
	{
		// Если контекст не активен, просто ничего не делаем, при создании контекста текстура
		// будет проинициализирована текстурным менеджером автоматически.
		if (!gl::ctx()->is_active()) return;
		// Нельзя повторно инициализировать текстуру
		if (m_initialized) {
			LOG(WARN) << "Повторная инициализация инициализированной текстуры!";
			return;
		}
		
		// Генерируем текстуру
		gl::ctx()->gen_textures(1, &m_id);
		
		// Привязываем текстуру к активному текстурному юниту
		gl::ctx()->bind_texture(m_id, m_unit);
		
		// Устанавливаем параметры текстуры
		gl::ctx()->set_tex_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		gl::ctx()->set_tex_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		gl::ctx()->set_tex_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		gl::ctx()->set_tex_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		// Формат изображения текстуры
		GLint format = 0;
		switch (image->components()) {
			case 1:
				format = GL_ALPHA;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			default:
				LOG(FATAL) << "Формат изображения для текстуры не поддерживается (" << image->components() << ")";
		}
		
		// Загружаем изображение в текстуру
		gl::ctx()->load_texture(GL_TEXTURE_2D, 0, format, image->width(), image->height(), 0, format, GL_UNSIGNED_BYTE, image->data());
		
		// Отмечаем, что текстура инициализирована
		m_initialized = true;
	}
	
	
	void Texture::deinitialize()
	{
		// Нельзя повторно освобождать текстуру
		if (!m_initialized) {
			LOG(WARN) << "Деинициализация не инициализированной текстуры!";
			return;
		}
		// Удаляем текстуру
		gl::ctx()->del_textures(1, &m_id);
		// Сбрасываем биндинг текстуры
		if (gl::ctx()->is_texture_binded(m_id, m_unit))
			gl::ctx()->bind_texture(0, m_unit);
		// Обнуляем идентификатор текстуры
		m_id = 0;
		// Отмечаем, что текстура деинициализирована
		m_initialized = false;
	}
	
	
	void Texture::use()
	{
		// Если текстура не инициализирована, то не может быть использована
		if (!m_initialized) return;
		// Привязываем текстуру к активному текстурному юниту
		gl::ctx()->bind_texture(m_id, m_unit);
	}
	
}
