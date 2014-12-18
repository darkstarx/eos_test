#include <memory/Cache.hpp>


namespace memory
{
	
	/** \class CacheDestroyer */
	
	CacheDestroyer::~CacheDestroyer()
	{
		ASSERT(m_instance);
		m_instance->m_instance = NULL;
		delete m_instance;
	}
	
	
	
	/** \class Cache */
	
	Cache* Cache::m_instance = NULL;
	CacheDestroyer Cache::m_destroyer;
	
	
	Cache& Cache::instance()
	{
		if (!m_instance)
		{
			m_instance = new Cache();
			m_destroyer.init(m_instance);
		}
		return *m_instance;
	}
	
	
	Cache::Cache()
	{
		LOG(INFO) << "Cache created";
	}
	
	
	Cache::~Cache()
	{
		LOG(INFO) << "Cache destroyed";
	}
	
}
