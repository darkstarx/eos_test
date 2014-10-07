#ifndef UTILS_BIND_HPP
#define UTILS_BIND_HPP

#include <functional>


namespace utils
{
	
	typedef std::function<void(void)> any_f;
	typedef std::function<void(bool)> bool_callback_t;
	typedef std::function<void(int)> int_callback_t;
	
}


#endif //UTILS_BIND_HPP
