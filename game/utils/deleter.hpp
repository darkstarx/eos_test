#ifndef UTILS_DELETER_HPP
#define UTILS_DELETER_HPP

#include <memory>


namespace utils
{
	
	template <typename T>
	inline void default_deleter(T * const ptr)
	{
		delete ptr;
	}
	
	template <typename T>
	inline void array_deleter(T * const ptr)
	{
		delete[] ptr;
	}
	
}


#endif // UTILS_DELETER_HPP
