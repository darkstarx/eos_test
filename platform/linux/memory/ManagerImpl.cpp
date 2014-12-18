#include <memory/Manager.hpp>
#include <utils/log.hpp>
#include <unistd.h>
#include <memory>


namespace memory
{
	
	void Manager::update()
	{
		static const int page_size = getpagesize();
		{
			m_used = 0;
			std::shared_ptr<FILE> fp(fopen("/proc/self/statm", "r"), fclose);
			ASSERT(fp);
			fscanf(fp.get(), "%*u %lu", &m_used);
			m_used *= page_size;
		}
		{
			m_total = 0;
			m_free = 0;
			std::shared_ptr<FILE> fp(fopen("/proc/meminfo", "r"), fclose);
			ASSERT(fp);
			fscanf(fp.get(), "%*s %lu %*s %*s %lu", &m_total, &m_free);
			m_total *= 1024;
			m_free *= 1024;
		}
	}
	
}
