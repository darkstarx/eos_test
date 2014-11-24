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
	
	
	int gettimeofday(struct timeval *tv, struct timezone *tz)
	{
		FILETIME ft;
		unsigned __int64 tmpres = 0;
		static int tzflag;
		
		if (tv != NULL)
		{
			GetSystemTimeAsFileTime(&ft);
		
			tmpres |= ft.dwHighDateTime;
			tmpres <<= 32;
			tmpres |= ft.dwLowDateTime;
		
			/* converting file time to unix epoch */
			tmpres -= DELTA_EPOCH_IN_MICROSECS; 
			tmpres /= 10;  /* convert into microseconds */
			tv->tv_sec = (long)(tmpres / 1000000UL);
			tv->tv_usec = (long)(tmpres % 1000000UL);
		}
		
		if (tz != NULL)
		{
			if (!tzflag)
			{
			_tzset();
			tzflag++;
			}
			tz->tz_minuteswest = _timezone / 60;
			tz->tz_dsttime = _daylight;
		}
		
		return 0;
	}
	
}

#endif // UTILS_TIME_IMPL_HPP