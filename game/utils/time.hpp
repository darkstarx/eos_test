#ifndef UTILS_TIME_HPP
#define UTILS_TIME_HPP

#include <sys/time.h>

#ifdef _WIN32
	#include <utils/time_impl.hpp>
#else
	#include <time.h>
#endif


namespace utils
{
	
	/** \brief Получить текущее время в секундах с начала эпохи (1970-01-01 00:00:00 +0000 (UTC))
	 */
	inline double get_time()
	{
#ifdef _WIN32
		timeval time;
		gettimeofday(&time, NULL);
		return static_cast<double>(time.tv_usec) / 1000000.0 + static_cast<double>(time.tv_sec);
#else
		timespec time;
		clock_gettime(CLOCK_MONOTONIC, &time);
		return static_cast<double>(time.tv_nsec) / 1000000000.0 + static_cast<double>(time.tv_sec);
#endif
	}
	
}

#endif // UTILS_TIME_HPP
