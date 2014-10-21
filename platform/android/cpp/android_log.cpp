#include "android_log.hpp"
#include <utils/log.hpp>
#include <utils/gettid.h>
#include <android/log.h>
#include <string>
#include <stdio.h>
#include <sys/stat.h>


namespace utils
{
	
	const log_level_t LL_INFO = ANDROID_LOG_INFO;
	const log_level_t LL_WARN = ANDROID_LOG_WARN;
	const log_level_t LL_ERROR = ANDROID_LOG_ERROR;
	const log_level_t LL_FATAL = ANDROID_LOG_FATAL;
	
	static const char *curr_log_filename = "curr_log.log"; //Должен совпадать с константой в Config.java
	static const char *last_log_filename ="last_log.log";
	static std::string directory_path = "";
	static FILE *android_log_file = NULL;
	
	void Log::log_to_syslog(const std::string &str) const
	{
		struct timeval tvnow;
		gettimeofday(&tvnow, NULL);
		struct tm tmnow;
		time_t tv_sec_tmp = tvnow.tv_sec;
		localtime_r(&tv_sec_tmp, &tmnow);
		{
			char buff[30];
			sprintf(buff, "%.2d:%.2d:%.2d.%.6ld  %d ",
				tmnow.tm_hour, tmnow.tm_min, tmnow.tm_sec, static_cast<long>(tvnow.tv_usec),
				gettid()
			);
			std::string msg(buff);
			__android_log_write(m_level, m_tag, msg.append(str).c_str());
		}
		
		if (android_log_file) {
			fprintf(
				android_log_file,
				"%.2d-%.2d %.2d:%.2d:%.2d.%.6ld %d %d %c %s\n",
				tmnow.tm_mon + 1, tmnow.tm_mday, tmnow.tm_hour, tmnow.tm_min, tmnow.tm_sec,
				static_cast<long>(tvnow.tv_usec),
				getpid(),
				gettid(),
				level_char(),
				str.c_str()
				);
		}
	}
	
	void Log::flush()
	{
		android_flush_log_file();
	}
	
	void android_set_log_directory_path(const std::string &directory_path_)
	{
		directory_path = directory_path_;
	}
	
	void android_create_log_file()
	{
		if (android_log_file) {
			fclose(android_log_file);
			android_log_file = NULL;
		}
		
		if (!directory_path.empty()) {
			const std::string curr_log_path = directory_path + "/" + curr_log_filename;
			const std::string last_log_path = directory_path + "/" + last_log_filename;
			
			struct stat buf;
			if (!stat(curr_log_path.c_str(), &buf)) {
				unlink(last_log_path.c_str());
				rename(curr_log_path.c_str(), last_log_path.c_str());
			}
			
			android_log_file = fopen(curr_log_path.c_str(), "w");
		}
	}
	
	void android_flush_log_file()
	{
		if (android_log_file) {
			fflush(android_log_file);
		}
	}
	
}
