#ifndef UTILS_GETTID_H
#define UTILS_GETTID_H


#if defined(_WIN32)

#include <windows.h>
typedef DWORD pid_t;

#else //_WIN32

#include <unistd.h>
#include <sys/syscall.h>

#endif //_WIN32


#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus


#if defined _WIN32

inline pid_t gettid(void)
{
	return GetCurrentThreadId();
}

#elif defined __APPLE__

inline pid_t gettid(void)
{
	return syscall(SYS_thread_selfid);
}

#elif defined ANDROID

inline pid_t gettid(void)
{
	return syscall(__NR_gettid);
}

#else

inline pid_t gettid(void)
{
	return syscall(SYS_gettid);
}

#endif


#ifdef  __cplusplus
}
#endif//__cplusplus


#endif//UTILS_GETTID_H
