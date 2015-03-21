#include <memory/Cache.hpp>
#include <utils/time.hpp>


namespace memory
{
	
	/** \struct Cache::CleanerS
	 * \brief Чистильщик кэша, удерживающего объекты
	 * Используется для определения, может ли объект в данный момент быть удален из кэша.
	 */
	template <class T>
	struct Cache::CleanerS
	{
		inline static bool process(const typename std::map<std::string, Cache::HolderS<T> >::value_type& p)
		{
			// Удаляем объект, если он не используется в течении некоторого времени
			static const time_t lifetime = sec_from_minutes(5);
			const time_t tnow(time(NULL));
			// Если объект используется, не удаляем его из кэша
			if (p.second.use_count() > 1) return false;
			// Если объект не использовался дольше установленного времени, удаляем его из кэша
			return (tnow - p.second.access_time() > lifetime);
		}
		inline static bool process_forced(const typename std::map<std::string, Cache::HolderS<T> >::value_type& p)
		{
			// Удаляем объект, если кроме кэша больше никто не удерживает этот объект
			return p.second.use_count() <= 1;
		}
	};
	
	
	
	/** \struct Cache::CleanerW
	 * \brief Чистильщик кэша, не удерживающего объекты 
	 * Используется для определения, может ли объект в данный момент быть удален из кэша.
	 */
	template <class T>
	struct Cache::CleanerW
	{
		inline static bool process(const typename std::map<std::string, Cache::HolderW<T> >::value_type& p)
		{
			// Удаляем объект, если он разрушен
			return p.second.expired();
		}
		inline static bool process_forced(const typename std::map<std::string, Cache::HolderW<T> >::value_type& p)
		{
			return process(p);
		}
	};
	
	
	
	/** \class Cache::HolderBase */
	
	inline Cache::HolderBase::HolderBase()
	: m_load_time(time(NULL))
	, m_access_time(m_load_time)
	{
	}
	
	
	inline time_t Cache::HolderBase::load_time() const
	{
		return m_load_time;
	}
	
	
	inline time_t Cache::HolderBase::access_time() const
	{
		return m_access_time;
	}
	
	
	inline void Cache::HolderBase::touch()
	{
		m_access_time = time(NULL);
	}
	
	
	
	/** \class Cache::HolderS */
	
	template <class TSPtr>
	inline Cache::HolderS<TSPtr>::HolderS(const TSPtr& org)
	: HolderBase()
	, m_value(org)
	{
	}
	
	
	template <class TSPtr>
	inline int Cache::HolderS<TSPtr>::use_count() const
	{
		return m_value.use_count();
	}
	
	
	template <class TSPtr>
	TSPtr& Cache::HolderS<TSPtr>::value()
	{
		return m_value;
	}
	
	
	template <class TSPtr>
	const TSPtr& Cache::HolderS<TSPtr>::value() const
	{
		return m_value;
	}
	
	
	
	/** \class Cache::HolderW */
	
	template <class TSPtr>
	inline Cache::HolderW<TSPtr>::HolderW()
	: HolderBase()
	{
	}
	
	
	template <class TSPtr>
	inline Cache::HolderW<TSPtr>::HolderW(const TSPtr& org)
	: HolderBase()
	, m_value(org)
	{
	}
	
	
	template <class TSPtr>
	inline int Cache::HolderW<TSPtr>::use_count() const
	{
		return m_value.use_count();
	}
	
	
	template <class TSPtr>
	TSPtr Cache::HolderW<TSPtr>::value() const
	{
		return m_value.lock();
	}
	
	
	
	/** \class Cache */
	
	Cache* Cache::m_instance = NULL;
	
	
	void Cache::create()
	{
		ASSERT(!m_instance) << "Повторное создание кэша!";
		m_instance = new Cache();
	}
	
	
	void Cache::destroy()
	{
		ASSERT(m_instance) << "Попытка разрушить не созданный кэш!";
		delete m_instance;
		m_instance = 0;
	}
	
	
	Cache& Cache::instance()
	{
		ASSERT(m_instance) << "Попытка обращения к отсутствующему кэшу!";
		return *m_instance;
	}
	
	
	template<>
	void Cache::cache_obj(const std::string& key, const utils::bytearray_sptr_t& obj)
	{
		cache_resource(key, obj, m_bytearrays);
	}
	
	
	template<>
	void Cache::cache_obj(const std::string& key, const graphics::TextureSPtr& obj)
	{
		cache_resource(key, obj, m_textures);
	}
	
	
	template<>
	utils::bytearray_sptr_t Cache::get_obj(const std::string& key)
	{
		const bytearray_cache_t::iterator it(cached_resource<utils::bytearray_sptr_t>(key, m_bytearrays));
		if (it != m_bytearrays.end()) {
			it->second.touch();
			return it->second.value();
		}
		return utils::bytearray_sptr_t();
	}
	
	
	template<>
	graphics::TextureSPtr Cache::get_obj(const std::string& key)
	{
		const texture_cache_t::iterator it(cached_resource<graphics::TextureSPtr>(key, m_textures));
		if (it != m_textures.end()) {
			it->second.touch();
			return it->second.value();
		}
		return graphics::TextureSPtr();
	}
	
	
	template <class TSPtr>
	void Cache::clean(const std::string& key)
	{
		m_textures.erase(key);
		m_bytearrays.erase(key);
	}
	
	
	template <>
	void Cache::clean<graphics::TextureSPtr>(const std::string& key)
	{
		m_textures.erase(key);
	}
	
	
	template <>
	void Cache::clean<utils::bytearray_sptr_t>(const std::string& key)
	{
		m_bytearrays.erase(key);
	}
	
	
	void Cache::clean()
	{
		do_clean(m_textures);
		do_clean(m_bytearrays);
	}
	
	
	void Cache::force_clean()
	{
		do_force_clean(m_textures);
		do_force_clean(m_bytearrays);
	}
	
	
	Cache::Cache()
	{
		LOG(INFO) << "Cache created";
	}
	
	
	Cache::~Cache()
	{
		LOG(INFO) << "Cache destroyed";
	}
	
	
	template <class HolderT>
	inline void Cache::do_clean(std::map<std::string, HolderT>& cache)
	{
		typedef typename std::map<std::string, HolderT>::iterator map_it_t;
		for (map_it_t it(cache.begin()), end(cache.end()); it != end; ) {
			if (HolderT::CleanerT::process(*it)) {
				cache.erase(it++);
				continue;
			}
			++it;
		}
	}
	
	
	template <class HolderT>
	inline void Cache::do_force_clean(std::map<std::string, HolderT>& cache)
	{
		typedef typename std::map<std::string, HolderT>::iterator map_it_t;
		for (map_it_t it(cache.begin()), end(cache.end()); it != end; )
		{
			if (HolderT::CleanerT::process_forced(*it)) {
				cache.erase(it++);
				continue;
			}
			++it;
		}
	}
	
	
	template <class T>
	inline typename std::map<std::string, Cache::HolderS<T> >::iterator Cache::cached_resource(const std::string& key, std::map<std::string, HolderS<T> >& cache)
	{
		typename std::map<std::string, Cache::HolderS<T> >::iterator it(cache.find(key));
		return it;
	}
	
	
	template <class T>
	inline typename std::map<std::string, Cache::HolderW<T> >::iterator Cache::cached_resource(const std::string& key, std::map<std::string, HolderW<T> >& cache)
	{
		typename std::map<std::string, HolderW<T> >::iterator it(cache.find(key));
		return it;
	}
	
	
	template <class T>
	inline const Cache::HolderS<T>& Cache::cache_resource(const std::string& key, const T& v, std::map<std::string, HolderS<T> >& cache)
	{
		ASSERT(v);
		// Нельзя позволять заменять значение при кэшировании
		const std::pair<typename std::map<std::string, HolderS<T> >::iterator, bool> r = cache.insert(std::make_pair(key, HolderS<T>(v)));
		if (!r.second) r.first->second.touch();
		return r.first->second;
	}
	
	
	template <class T>
	inline const Cache::HolderW<T>& Cache::cache_resource(const std::string& key, const T& v, std::map<std::string, HolderW<T> >& cache)
	{
		ASSERT(v);
		// Нельзя позволять заменять значение при кэшировании
		const HolderW<T> holder(v);
		typename std::map<std::string, HolderW<T> >::iterator it(cache.insert(std::make_pair(key, holder)).first);
		if (it->second.expired()) it->second = holder;
		return it->second;
	}
	
}
