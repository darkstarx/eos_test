#include <memory/Manager.hpp>


namespace memory
{
	
	Manager* Manager::m_instance = NULL;
	
	
	void Manager::create()
	{
		ASSERT(!m_instance) << "Повторное создание менеджера памяти!";
		m_instance = new Manager();
	}
	
	
	void Manager::destroy()
	{
		ASSERT(m_instance) << "Ошибка при разрушении менеджера памяти: экземпляр не создан!";
		delete m_instance;
		m_instance = 0;
	}
	
	
	Manager& Manager::instance()
	{
		ASSERT(m_instance) << "Ошибка при обращении к менеджеру памяти: экземпляр не создан!";
		return *m_instance;
	}
	
	
	Manager::Manager()
	: m_total(0)
	, m_free(0)
	, m_free_limit(0)
	, m_used(0)
	{
		LOG(INFO) << "Memory manager created";
	}
	
	
	Manager::~Manager()
	{
		LOG(INFO) << "Memory manager destroyed";
	}
	
}
