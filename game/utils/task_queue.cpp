#include <utils/task_queue.hpp>
#include <utils/log.hpp>
#include <utils/time.hpp>


namespace utils
{
	
	/** \brief Данные задачи в очереди.
	 */
	struct task_t
	{
		/**
		 * Не позволяем копировать
		 */
		task_t(const task_t&) = delete;
		
		/**
		 * Не позволяем присваивать
		 */
		task_t& operator=(const task_t&) = delete;
		
		task_t(const task_queue::task_f &task);
		
		task_t(const task_queue::task_f &task, const double interval);
		
		bool is_repeating() const;
		
		bool is_enabled() const;
		
		void disable();
		
		double abstime;				///< Время выполнения
		double interval;			///< Интервал повторения
		task_queue::task_f task;	///< Функтор
	};
	
	
	inline task_t::task_t(const task_queue::task_f &task)
	: abstime(0)
	, interval(-1)
	, task(task)
	{
	}
	
	
	inline task_t::task_t(const task_queue::task_f &task, const double interval)
	: abstime(0)
	, interval(interval)
	, task(task)
	{
	}
	
	
	inline bool task_t::is_repeating() const
	{
		return interval >= 0;
	}
	
	
	
	/** \class task_queue */
	
	task_queue::task_queue()
	: m_tvnow(0)
	, m_tvend(0)
	, m_tvtmp(0)
	, m_max_process_time(0)
	{
	}
	
	
	task_queue::~task_queue()
	{
	}
	
	
	void task_queue::process_queue()
	{
		scoped_lock queue_guard(m_queue_lock);
		scoped_lock hot_guard(m_hot_lock);
		
		if (m_queue.empty() && m_hot.empty()) return;
		
		m_tvnow = get_time();
		m_tvend = m_tvnow + m_max_process_time;
		
		for (bool timeout = false; !timeout; m_tvnow = get_time())
		{
			{
				// Перекладываем задачи в список исполнения
				queue_t::iterator begin(m_queue.begin()), it(begin), end(m_queue.end());
				for (; it != end && it->first <= m_tvnow; ++it) {
					m_hot.push_back(it->second);
				}
				m_queue.erase(begin, it);
			}
			if (m_hot.empty()) break;
			
			m_queue_lock.unlock();
			{
				// Обрабатываем список исполнения
				tasks_t::iterator begin(m_hot.begin()), it(begin), end(m_hot.end());
				for (; it != end; ++it) {
					m_tvtmp = get_time();
					if (m_tvend <= m_tvtmp) {
						break;
					}
					{
						task_sptr_t &task(*it);
						m_hot_lock.unlock();
						process_task(task);
						m_hot_lock.lock();
					}
				}
				m_hot.erase(begin, it);
			}
			m_queue_lock.lock();
			break;
		}
	}
	
	
	void task_queue::set_max_process_time(const double usec)
	{
		ASSERT(usec >= 0);
		m_max_process_time = usec / 1000000.0;
	}
	
	
	task_wptr_t task_queue::enqueue_simple(const task_f &task)
	{
		return enqueue(get_time(), task_sptr_t(new task_t(task)));
	}
	
	
	task_wptr_t task_queue::enqueue_oneshot(const task_f &task, const double delay)
	{
		return enqueue(get_time() + delay, task_sptr_t(new task_t(task)));
	}
	
	
	task_wptr_t task_queue::enqueue_repeatedly_delayed(const task_f &task, const double delay, const double interval)
	{
		return enqueue(get_time() + delay, task_sptr_t(new task_t(task, interval)));
	}
	
	
	bool task_queue::unqueue(const task_wptr_t &ident)
	{
		const task_sptr_t task(ident.lock());
		if (!task) return false;
		{
			const scoped_lock guard(m_queue_lock);
			for (queue_t::iterator it(m_queue.begin()), end(m_queue.end()); it != end && it->first <= task->abstime; ++it) {
				if (it->second == task) {
					m_queue.erase(it);
					return true;
				}
			}
		}
		{
			const scoped_lock guard(m_hot_lock);
			for (tasks_t::iterator it(m_hot.begin()), end(m_hot.end()); it != end && (!*it || (*it)->abstime <= task->abstime); ++it) {
				if (*it == task) {
					it->reset();
					return true;
				}
			}
		}
		return false;
	}
	
	
	void task_queue::unqueue_all()
	{
		{
			const scoped_lock guard(m_queue_lock);
			m_queue.clear();
		}
		{
			const scoped_lock guard(m_hot_lock);
			m_hot.clear();
		}
	}
	
	
	task_wptr_t task_queue::enqueue(const double abstime, const task_sptr_t &task)
	{
		task->abstime = abstime;
		return enqueue(task);
	}
	
	
	task_wptr_t task_queue::enqueue(const task_sptr_t &task)
	{
		{
			const scoped_lock guard(m_queue_lock);
			m_queue.insert(std::make_pair(task->abstime, task));
		}
		return task;
	}
	
	
	void task_queue::process_task(task_sptr_t &task)
	{
		execute_task(task);
		
		// За время выполнения задача уже может быть аннулирована.
		const task_sptr_t task_(task);
		if (!task_) return;
		task.reset();
		
		if (!task_->is_repeating()) return;
		
		double &tvnext = task_->abstime;
		tvnext = tvnext + task_->interval;
		if (tvnext <= m_tvnow) {
			tvnext = m_tvnow + task_->interval;
		}
		
		enqueue(task_);
	}
	
	
	void task_queue::execute_task(task_sptr_t &task)
	{
		task_f task_func;
		{
			const task_sptr_t task_(task);
			if (!task_) return;
			task_func = task_->task;
			if (!task_->is_repeating()) {
				task.reset();
				// Если у задачи нет больше повторений, очищаем её функтор, чтобы его ресурсы очистились сразу после выполнения
				task_->task = task_f();
			}
		}
		task_func();
	}
	
}
