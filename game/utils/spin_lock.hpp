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
			lck.clear();
		}
		void lock()
		{
			while (lck.test_and_set(std::memory_order_acquire)) ;
		}
		
		void unlock()
		{
			lck.clear(std::memory_order_release);
		}
		
	private:
		std::atomic_flag lck;
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
