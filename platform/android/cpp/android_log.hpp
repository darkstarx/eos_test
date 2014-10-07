#ifndef ANDROID_LOG_HPP
#define ANDROID_LOG_HPP

#include <string>


namespace utils
{
	void android_set_log_directory_path(const std::string &directory_path);
	void android_create_log_file();
	void android_flush_log_file();
}

#endif //ANDROID_LOG_HPP
