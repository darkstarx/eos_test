#include <graphics/TextureManager.hpp>
#include <graphics/Texture.hpp>
#include <resources/Image.hpp>
#include <resources/ImageLoader.hpp>
#include <memory/Cache.hpp>


namespace graphics
{
	
	/** \class TextureManagerDestroyer */
	
	TextureManagerDestroyer::~TextureManagerDestroyer()
	{
		ASSERT(m_instance);
		m_instance->m_instance = NULL;
		delete m_instance;
	}
	
	
	
	/** \class TextureManager */
	
	TextureManager* TextureManager::m_instance = NULL;
	TextureManagerDestroyer TextureManager::m_destroyer;
	
	
	TextureManager& TextureManager::instance()
	{
		if (!m_instance)
		{
			m_instance = new TextureManager();
			m_destroyer.init(m_instance);
		}
		return *m_instance;
	}
	
	
	TextureSPtr TextureManager::get_texture_from_asset(const std::string &filename)
	{
		// Ключ кэширования
		const std::string cache_key("ta:" + filename);
		// Пытаемся найти текстуру в кэше
		graphics::TextureSPtr tex = cache().get_obj<graphics::TextureSPtr>(cache_key);
		// Если текстура найдена, возвращаем её
		if (tex) return tex;
		// Создаем объект текстуры
		tex.reset(new graphics::Texture());
		// Пытаемся загрузить изображение из ассетов
		resources::ImageSPtr img = ImageLoader::load_from_asset(filename);
		// Если не удалось загрузить изображение и из ассетов, то оставляем текстуру не инициализированной
		if (!img) {
			LOG(ERR) << "Не удалось загрузить из ассетов изображение для текстуры `" << filename << "`";
		} else {
			tex->initialize(img);
			m_texture_sources[tex.get()].asset_name = filename;
			// Кэшируем текстуру
			cache().cache_obj(cache_key, tex);
		}
		// Запоминаем указатель на текстуру
		m_textures.push_back(TextureWPtr(tex));
		// Возвращаем объект текстуры
		return tex;
	}
	
	
	TextureSPtr TextureManager::get_texture_from_storage(const std::string &filename)
	{
		// Ключ кэширования
		const std::string cache_key("fa:" + filename);
		// Пытаемся найти текстуру в кэше
		graphics::TextureSPtr tex = cache().get_obj<graphics::TextureSPtr>(cache_key);
		// Если текстура найдена, возвращаем её
		if (tex) return tex;
		// Создаем объект текстуры
		tex.reset(new graphics::Texture());
		// Пытаемся загрузить изображение из ресурсов
		resources::ImageSPtr img = ImageLoader::load_from_res(filename);
		// Если не удалось загрузить изображение и из ресурсов, то оставляем текстуру не инициализированной
		if (!img) {
			LOG(ERR) << "Не удалось загрузить из ресурсов изображение для текстуры `" << filename << "`";
		} else {
			tex->initialize(img);
			m_texture_sources[tex.get()].res_name = filename;
			// Кэшируем текстуру
			cache().cache_obj(cache_key, tex);
		}
		// Запоминаем указатель на текстуру
		m_textures.push_back(TextureWPtr(tex));
		// Возвращаем объект текстуры
		return tex;
	}
	
	
	void TextureManager::unload()
	{
		for (textures_t::iterator it = m_textures.begin(), end = m_textures.end(); it != end; ++it) {
			TextureSPtr texture = it->lock();
			if (!texture) continue;
			texture->deinitialize();
		}
	}
	
	
	void TextureManager::reload()
	{
		for (textures_t::iterator it = m_textures.begin(), end = m_textures.end(); it != end; ++it) {
			TextureSPtr texture = it->lock();
			// Если текстура уже разрушена или уже загружена, то пропускаем её
			if (!texture || texture->initialized()) continue;
			// Ищем источник изображения для перезагрузки текстуры
			texture_sources_t::iterator it_src = m_texture_sources.find(texture.get());
			// Если источник изображения не удалось найти
			if (it_src == m_texture_sources.end()) {
				LOG(ERR) << "Не удалось определить источник изображения для текстуры!";
				continue;
			}
			const texture_source &src = it_src->second;
			// Если источник это ассет
			if (!src.asset_name.empty()) {
				// Пытаемся загрузить изображение из ассетов
				resources::ImageSPtr img = ImageLoader::load_from_asset(src.asset_name);
				// Если не удалось загрузить изображение и из ассетов, то оставляем текстуру не инициализированной
				if (img) {
					texture->initialize(img);
				} else {
					LOG(ERR) << "Не удалось загрузить из ассетов изображение для текстуры `" << src.asset_name << "`";
				}
			} else
			// Если источник это ресурс из хранилища
			if (!src.res_name.empty()) {
				// Пытаемся загрузить изображение из ресурсов
				resources::ImageSPtr img = ImageLoader::load_from_res(src.res_name);
				// Если не удалось загрузить изображение и из ресурсов, то оставляем текстуру не инициализированной
				if (img) {
					texture->initialize(img);
				} else {
					LOG(ERR) << "Не удалось загрузить из ресурсов изображение для текстуры `" << src.res_name << "`";
				}
			} else {
				// Если ресурс не определен, то текстуру не удастся перезагрузить
				LOG(ERR) << "Не удалось определить источник изображения для текстуры!";
			}
		}
	}
	
	
	void TextureManager::release(TexturePtr texture)
	{
		for (textures_t::iterator it = m_textures.begin(); it != m_textures.end(); ) {
			TextureSPtr tex = it->lock();
			if (!tex || tex.get() == texture) {
				m_textures.erase(it++);
				continue;
			}
			++it;
		}
		m_texture_sources.erase(texture);
	}
	
	
	TextureManager::TextureManager()
	{
		LOG(INFO) << "Texture manager created";
	}
	
	
	TextureManager::~TextureManager()
	{
		LOG(INFO) << "Texture manager destroyed";
	}
	
}
