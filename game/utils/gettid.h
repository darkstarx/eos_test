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


#ifndef linux

inline pid_t gettid(void)
{
	#ifdef  __APPLE__
	return syscall(SYS_thread_selfid);
	#endif//__APPLE__
	#ifdef  _WIN32
	return GetCurrentThreadId();
	#endif//_WIN32
}

#else //linux

#ifndef ANDROID

inline pid_t gettid(void)
{
	return syscall(SYS_gettid);
}

#endif

#endif//linux

#ifdef  __cplusplus
}
#endif//__cplusplus


#endif//UTILS_GETTID_H
