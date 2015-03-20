#ifndef UTILS_TIME_IMPL_HPP
#define UTILS_TIME_IMPL_HPP

#include <time.h>
#include <windows.h>
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif


namespace utils
{
	
	struct timezone 
	{
	int  tz_minuteswest; /* minutes W of Greenwich */
	int  tz_dsttime;     /* type of dst correction */
	};
	
	int gettimeofday(struct timeval *tv, struct timezone *tz);
	
}

#endif // UTILS_TIME_IMPL_HPP
