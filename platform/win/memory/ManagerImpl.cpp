#include <memory/Manager.hpp>
#include <Windows.h>


namespace memory
{
	
	void Manager::update()
	{
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);
		m_total = memInfo.ullTotalPhys;
		m_free = memInfo.ullAvailPhys;
		m_used = m_total - m_free;
	}
	
}
