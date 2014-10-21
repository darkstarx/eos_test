#ifndef UTILS_LOG_HPP
#define UTILS_LOG_HPP

#include <sstream>
#include <stdexcept>


namespace utils
{
	
	typedef int log_level_t;
	
	extern const log_level_t LL_INFO;
	extern const log_level_t LL_WARN;
	extern const log_level_t LL_ERROR;
	extern const log_level_t LL_FATAL;
	
#define INFO ::utils::LL_INFO
#define WARN ::utils::LL_WARN
#define ERR ::utils::LL_ERROR
#define FATAL ::utils::LL_FATAL
	
	
	class Log
	{
	public:
		explicit Log(const log_level_t level);
		static void flush();
		Log(const log_level_t level, const char *file, unsigned int line);
		~Log();
		
		inline std::ostringstream& stream() const
		{ return *m_oss; }
		
	private:
		struct ostringstream : public std::ostringstream {};
		
		const log_level_t m_level;
		ostringstream * const m_oss;
		
		static const char * const m_tag;
		
		char level_char() const;
		void log_to_syslog(const std::string &str) const;
	};
	
	struct Voidify { inline void operator &(const std::ostream&) const { } };
	
	std::string posix_error_string();
	std::string posix_error_string(int errcode);
	
}


#define LOG(level) ::utils::Log(level, __FILE__, __LINE__).stream()
#define LOG_IF(level, cond) !(cond) ? (void)0 : ::utils::Voidify() & ::utils::Log(level, __FILE__, __LINE__).stream()

#define CHECK(cond) (cond) ? (void)0 : ::utils::Voidify() & ::utils::Log(FATAL, __FILE__, __LINE__).stream()
#define CHECK2(cond) (cond) ? (void)0 : ::utils::Voidify() & ::utils::Log(FATAL, __FILE__, __LINE__).stream() << # cond

#define PCHECK(cond) CHECK(cond) << utils::posix_error_string() << ": "
#define PCHECK2(cond, errsrc) CHECK(cond) << utils::posix_error_string(errsrc) << ": "

#define LOG_VOIDIFY() true ? (void)0 : ::utils::Voidify() & ::utils::Log(FATAL, __FILE__, __LINE__).stream()

#ifndef NDEBUG
	
	#define DCHECK(cond) CHECK(cond)
	#define ASSERT(cond) CHECK(cond) << "Assertion failed: " << # cond
	
	#define DLOG(level) LOG(level)
	#define DLOG_IF(level, cond) LOG_IF(level, cond)
	
#else //NDEBUG
	
	#define DCHECK(cond) LOG_VOIDIFY()
	#define ASSERT(cond) LOG_VOIDIFY()
	
	#define DLOG(level) LOG_VOIDIFY()
	#define DLOG_IF(level, cond) LOG_VOIDIFY()
	
#endif //NDEBUG


#endif //UTILS_LOG_HPP
