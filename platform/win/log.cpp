#include <utils/log.hpp>
#include <utils/gettid.h>
#include <Windows.h>
#include <iostream>
#include <iomanip>

namespace utils
{
	
	const log_level_t LL_INFO = 0;
	const log_level_t LL_WARN = 1;
	const log_level_t LL_ERROR = 2;
	const log_level_t LL_FATAL = 3;
	
	
	void Log::log_to_syslog(const std::string& str) const
	{
		std::ostringstream stream;
		
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		{
			char buff[40];
			sprintf_s(buff, "%.2d:%.2d:%.2d.%.6ld  %d %d %c ",
				tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds,
				GetCurrentProcessId(), GetCurrentThreadId(), level_char());
			stream << buff;
		}
		stream << str << std::endl;
		const std::string msg = stream.str();
		OutputDebugString(msg.c_str());
	}
	
	void Log::flush()
	{
	}
	
}
