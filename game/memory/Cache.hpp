#ifndef MEMORY_CACHE_HPP
#define MEMORY_CACHE_HPP

#include <graphics/forwards.hpp>
#include <utils/log.hpp>
#include <list>


namespace memory
{
	
	class Cache;
	
	
	class CacheDestroyer
	{
	public:
		~CacheDestroyer();
		inline void init(Cache *instance) { ASSERT(!m_instance); m_instance = instance; }
	private:
		Cache *m_instance;
	};
	
	
	/** \brief Кэш объектов
	 */
	class Cache
	{
	private:
		friend class CacheDestroyer;
		
		typedef std::list<graphics::TexturePtr> texture_cache_t;
		
	public:
		Cache(const Cache& root) = delete;
		
		Cache& operator=(const Cache&) = delete;
		
		static bool is_alive() { return m_instance; }
		
		static Cache& instance();
		
	private:
		static Cache* m_instance;			///< Единственный экземпляр отрисовщика
		static CacheDestroyer m_destroyer;	///< Разрушитель экземпляра рендерера
		texture_cache_t m_textures;			///< Кэш текстур
		
		Cache();
		
		~Cache();
	};
	
}

inline memory::Cache& cache() { return memory::Cache::instance(); }

#endif // MEMORY_CACHE_HPP
