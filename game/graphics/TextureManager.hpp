#ifndef GRAPHICS_TEXTUREMANAGER_HPP
#define GRAPHICS_TEXTUREMANAGER_HPP

#include <graphics/forwards.hpp>
#include <resources/forwards.hpp>
#include <utils/log.hpp>
#include <list>
#include <map>


namespace graphics
{
	
	class TextureManager;
	
	/** \brief Вспомогательный класс для автоматического разрушения текстурного менеджера
	 */
	class TextureManagerDestroyer
	{
	public:
		~TextureManagerDestroyer();
		inline void init(TextureManager *instance) { ASSERT(!m_instance); m_instance = instance; }
	private:
		TextureManager *m_instance;
	};
	
	/** \brief Источник изображения для текстуры
	 */
	struct texture_source
	{
		std::string asset_name;	///< Имя файла из ассетов
		std::string res_name;	///< Имя файла из хранилища ресурсов
	};
	
	/** \brief Текстурный менеджер
	 */
	class TextureManager
	{
	private:
		friend class TextureManagerDestroyer;
		typedef std::list<TextureWPtr> textures_t;
		typedef std::map<TexturePtr, texture_source> texture_sources_t;
		
	public:
		TextureManager(const TextureManager& root) = delete;
		
		TextureManager& operator=(const TextureManager&) = delete;
		
		static bool is_alive() { return m_instance != 0; }
		
		static TextureManager& instance();
		
		/** \brief Получить текстуру по имени ассета
		 */
		TextureSPtr get_texture_from_asset(const std::string &filename);
		
		/** \brief Получить текстуру по имени файла
		 */
		TextureSPtr get_texture_from_storage(const std::string &filename);
		
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
		static TextureManagerDestroyer m_destroyer;	///< Разрушитель экземпляра менеджера
		
		textures_t m_textures;						///< Список слабых указателей на все объекты текстур
		texture_sources_t m_texture_sources;		///< Источники изображений для текстур
		
		TextureManager();
		
		~TextureManager();
	};
	
}

inline graphics::TextureManager& texmgr() { return graphics::TextureManager::instance(); }

#endif // GRAPHICS_TEXTUREMANAGER_HPP
