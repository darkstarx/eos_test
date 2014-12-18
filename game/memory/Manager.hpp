#ifndef MEMORY_MANAGER_HPP
#define MEMORY_MANAGER_HPP

#include <utils/log.hpp>


namespace memory
{
	
	/** \brief Менеджер памяти
	 * Должен создаваться при создании приложения и разрушаться при разрушении приложения. Должен
	 * существовать только в единичном экземпляре, копирование и присваивание запрещается.
	 */
	class Manager
	{
	public:
		Manager(const Manager& copy) = delete;
		
		Manager& operator=(const Manager&) = delete;
		
		static void create();
		
		static void destroy();
		
		static bool is_alive() { return m_instance; }
		
		static Manager& instance();
		
		/** \brief Обновить сведения об использовании памяти
		 */
		void update();
		
	private:
		static Manager* m_instance;				///< Единственный экземпляр менеджера
		
		size_t m_total;							///< Общее кол-во памяти в устройстве
		size_t m_free;							///< Кол-во свободной памяти, доступной для приложения
		size_t m_free_limit;					///< Минимальное кол-во свободной памяти для нормальной работы приложения
		size_t m_used;							///< Кол-во занятой приложением памяти
		
		Manager();
		
		~Manager();
	};
	
	inline Manager& mgr() { return Manager::instance(); }
	
}

#endif // MEMORY_MANAGER_HPP
