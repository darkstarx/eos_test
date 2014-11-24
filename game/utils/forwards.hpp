#ifndef UTILS_FORWARDS_HPP
#define UTILS_FORWARDS_HPP

#include <memory>


namespace utils
{
	
	class task_queue;
	typedef std::shared_ptr<task_queue> task_queue_sptr;
	
	struct task_t;
	typedef std::shared_ptr<task_t> task_sptr_t;
	typedef std::weak_ptr<task_t> task_wptr_t;
	
}

#endif // UTILS_FORWARDS_HPP
