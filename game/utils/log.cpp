#include <utils/log.hpp>
#include <utils/libgen.hpp>
#include <string.h>
#include <errno.h>
#include <stdexcept>

#ifndef LOG_TAG
	#error "Please, define LOG_TAG"
	#define LOG_TAG "log_tag"
#endif //LOG_TAG


namespace utils
{
	
	const char * const Log::m_tag = LOG_TAG;
	
	
	Log::Log(const log_level_t level)
	: m_level(level)
	, m_oss(new ostringstream)
	{
	}
	
	
	Log::Log(const log_level_t level, const char *file, unsigned int line)
	: m_level(level)
	, m_oss(new ostringstream)
	{
		*m_oss << (file ? base_name(file) : "unknown");
		if (line) *m_oss << ':' << line;
		*m_oss << "] ";
	}
	
	
	Log::~Log()
	{
		const std::string str(m_oss->str());
		delete m_oss;
		
		log_to_syslog(str);
		
		if (m_level == FATAL)
			throw std::runtime_error(str);
	}
	
	
	char Log::level_char() const
	{
		if (m_level == INFO) return 'I';
		if (m_level == WARN) return 'W';
		if (m_level == ERR) return 'E';
		if (m_level == FATAL) return 'F';
		return '?';
	}
	
	
	
	const char * posix_error_string()
	{
		return posix_error_string(errno);
	}
	
	
	const char * posix_error_string(int errcode)
	{
		return strerror(errcode);
	}
	
}
