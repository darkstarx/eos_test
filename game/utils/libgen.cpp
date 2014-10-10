#include <utils/libgen.hpp>
#include <libgen.h>
#include <string.h>

#ifndef MAX_PATH

#ifdef  PATH_MAX
#define MAX_PATH PATH_MAX
#else //PATH_MAX
#define MAX_PATH 255
#endif//PATH_MAX

#endif//MAX_PATH


namespace utils
{
	
	//std::string dir_name(const char *_path)
	//{
	//	char path[MAX_PATH + 1];
	//	path[MAX_PATH] = 0;
	//	strncpy(path, _path, MAX_PATH);
	//	return std::string(dirname(path));
	//}
	
	std::string base_name(const char *_path)
	{
		char path[MAX_PATH + 1];
		path[MAX_PATH] = 0;
#ifdef _WIN32
		strncpy_s(path, _path, MAX_PATH);
#else
		strncpy(path, _path, MAX_PATH);
#endif
		return std::string(basename(path));
	}
	
}
