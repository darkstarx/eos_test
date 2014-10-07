#include <utils/log.hpp>
#include <utils/gettid.h>
#include <iostream>
#include <sys/time.h>

namespace utils
{
	
	const log_level_t LL_INFO = 0;
	const log_level_t LL_WARN = 1;
	const log_level_t LL_ERROR = 2;
	const log_level_t LL_FATAL = 3;
	
	
	void Log::log_to_syslog(const std::string& str) const
	{
		struct timeval tvnow;
		gettimeofday(&tvnow, NULL);
		
		struct tm tmnow;
		time_t tv_sec_tmp = tvnow.tv_sec;
		localtime_r(&tv_sec_tmp, &tmnow);
		printf(
			"%.2d:%.2d:%.2d.%.6ld %d %d %c %s\n",
			tmnow.tm_hour, tmnow.tm_min, tmnow.tm_sec,
			static_cast<long>(tvnow.tv_usec),
			getpid(),
			gettid(),
			level_char(),
			str.c_str()
			);
		fflush(stdout);
	}
	
	void Log::flush()
	{
	}
	
}
