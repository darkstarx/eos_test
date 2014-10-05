#ifndef WINLOG_HPP
#define	WINLOG_HPP

#include "log.h"
#include <iomanip>


class WinDebugSink : public google::LogSink
{
	virtual void send(google::LogSeverity severity, const char* full_filename,
		const char* base_filename, int line,
        const struct ::tm* tm_time,
        const char* message, size_t message_len)
	{
		//std::string msg = ToString(severity, full_filename, line, tm_time, message, message_len);
		std::ostringstream stream(std::string(message, message_len));
		stream.fill('0');
		int usecs = 0;

		stream << google::LogSeverityNames[severity][0]
				<< std::setw(2) << 1+tm_time->tm_mon
				<< std::setw(2) << tm_time->tm_mday
				<< ' '
				<< std::setw(2) << tm_time->tm_hour << ':'
				<< std::setw(2) << tm_time->tm_min << ':'
				<< std::setw(2) << tm_time->tm_sec << '.'
				<< std::setw(6) << usecs
				<< ' '
				<< std::setfill(' ') << std::setw(5) << GetCurrentThreadId() << std::setfill('0')
				<< ' '
				<< base_filename << ':' << line << "] ";

		stream << std::string(message, message_len) << std::endl;
		const std::string msg = stream.str();
		OutputDebugString(msg.c_str());
	}
};

#endif //WINLOG_HPP