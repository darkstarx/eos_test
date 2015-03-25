#ifndef GRAPHICS_TEXTUREMANAGER_HPP
#define GRAPHICS_TEXTUREMANAGER_HPP

#include <graphics/forwards.hpp>
#include <resources/forwards.hpp>
#include <utils/log.hpp>
#include <list>
#include <map>


namespace graphics
{
	
	/** \brief Источник изображения для текстуры
	 */
	struct texture_source
	{
		std::string asset_name;			///< Имя файла из ассетов
		std::string res_name;			///< Имя файла из хранилища ресурсов
		resources::ImageSPtr image;		///< Изображение для загрузки в текстуру
	};
	
	/** \brief Текстурный менеджер
	 */
	class TextureManager
	{
	private:
		typedef std::list<TextureWPtr> textures_t;
		typedef std::map<TexturePtr, texture_source> texture_sources_t;
		
	public:
		TextureManager(const TextureManager& root) = delete;
		
		TextureManager& operator=(const TextureManager&) = delete;
		
		/** \brief Создать текстурный менеджер
		 */
		static void create();
		
		/** \brief Разрушить текстурный менеджер
		 */
		static void destroy();
		
		/** \brief Проверить, что текстурный менеджер создан
		 */
		static bool is_alive() { return m_instance != 0; }
		
		/** \brief Получить экземпляр текстурного менеджера
		 */
		static TextureManager& instance();
		
		/** \brief Получить текстуру по имени ассета
		 */
		TextureSPtr get_texture_from_asset(const std::string &filename);
		
		/** \brief Получить текстуру по имени файла
		 */
		TextureSPtr get_texture_from_storage(const std::string &filename);
		
		/** \brief Создать пустую текстуру, заполненную нулями
		 */
		TextureSPtr create_empty_texture(unsigned int width, unsigned int height, unsigned char components);
		
		/** \brief Выгрузить все текстуры и освободить память
		 */
		void unload();
		
		/** \brief Перезагрузить все текстуры, которые еще не загружены
		 */
		void reload();
		
		/** \brief Освободить текстуру из-под управления менеджером
		 */
		void release(TexturePtr texture);
		
	private:
		static TextureManager* m_instance;			///< Единственный экземпляр менеджера
		
		textures_t m_textures;						///< Список слабых указателей на все объекты текстур
		texture_sources_t m_texture_sources;		///< Источники изображений для текстур
		
		TextureManager();
		
		~TextureManager();
	};
	
}

inline graphics::TextureManager& texmgr() { return graphics::TextureManager::instance(); }

#endif // GRAPHICS_TEXTUREMANAGER_HPP
