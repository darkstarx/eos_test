#ifndef SPIN_LOCK_H
#define SPIN_LOCK_H

#include <atomic>


namespace utils
{
	
	class spin_lock
	{
	public:
		spin_lock()
		{
			m_lock.clear();
		}
		
		void lock()
		{
			while (m_lock.test_and_set(std::memory_order_acquire)) ;
		}
		
		void unlock()
		{
			m_lock.clear(std::memory_order_release);
		}
		
	private:
		std::atomic_flag m_lock;
	};
	
	
	class scoped_lock
	{
	public:
		scoped_lock(spin_lock &lock) : m_lock(lock) { m_lock.lock(); }
		~scoped_lock() { m_lock.unlock(); }
		
	private:
		spin_lock &m_lock;
	};
	
}

#endif // SPIN_LOCK_H
