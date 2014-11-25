#ifndef UTILS_TO_STRING_HPP
#define UTILS_TO_STRING_HPP

#include <string>
#ifdef __ANDROID__
	#include <sstream>
#endif


namespace utils
{
	
	template <typename T>
	std::string to_string(T value)
	{
#ifdef __ANDROID__
		std::ostringstream os ;
		os << value ;
		return os.str() ;
#else
		return std::to_string(value);
#endif
	}
	
}

#endif //UTILS_TO_STRING_HPP