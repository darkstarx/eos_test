#ifndef UTILS_TASK_QUEUE_HPP
#define UTILS_TASK_QUEUE_HPP

#include <utils/forwards.hpp>
#include <utils/spin_lock.hpp>
#include <functional>
#include <vector>
#include <map>


namespace utils
{
	
	class task_queue
	{
	public:
		typedef std::function<void(void)> task_f;
		
	private:
		typedef std::multimap<double, task_sptr_t> queue_t;
		typedef std::vector<task_sptr_t> tasks_t;
		
	public:
		task_queue();
		
		~task_queue();
		
		/** \brief Выполнить обработку очереди
		 */
		void process_queue();
		
		/** \brief Установить максимальное время обработки очереди
		 * \param value Допустимое время обработки очереди, в миллисекундах.
		 */
		void set_max_process_time(double value);
		
		/** \brief Добавить однократно выполняемую задачу
		 * \param task Функтор задачи.
		 * \return Возвращает объект задачи.
		 */
		task_wptr_t enqueue_simple(const task_f &task);
		
		/** \brief Добавить задачу, выполняемую однократно с задержкой
		 * \param task Функтор задачи.
		 * \param delay Задержка перед выполнением задачи.
		 * \return Возвращает объект задачи.
		 */
		task_wptr_t enqueue_oneshot(const task_f &task, const double delay);
		
		/** \brief Добавить задачу, выполняемую периодически
		 * \param task Функтор задачи.
		 * \param interval Интервал повторения задачи.
		 * \return Возвращает объект задачи.
		 */
		task_wptr_t enqueue_repeatedly(const task_f &task, const double interval);
		
		/** \brief Добавить задачу, выполняемую периодически с первоначальной задержкой
		 * \param task Функтор задачи.
		 * \param delay Задержка перед выполнением задачи.
		 * \param interval Интервал повторения задачи.
		 * \return Возвращает объект задачи.
		 */
		task_wptr_t enqueue_repeatedly_delayed(const task_f &task, const double delay, const double interval);
		
		/** \brief Удалить задачу из очереди выполнения
		 * \param task Объект задачи.
		 * \return Признак успешности удаления задачи (задача была в очереди).
		 */
		bool unqueue(const task_wptr_t &task);
		
		/** \brief Удалить все задачи из очереди
		 */
		void unqueue_all();
		
	private:
		queue_t m_queue;
		spin_lock m_queue_lock;
		tasks_t m_hot;
		spin_lock m_hot_lock;
		double m_tvnow;
		double m_tvend;
		double m_tvtmp;
		double m_max_process_time;
		
		/** \brief Добавить задачу в очередь с установкой времени её выполнения
		 * \param abstime Время выполнения задачи.
		 * \param task Задача.
		 * \return Возвращает объект задачи.
		 */
		task_wptr_t enqueue(const double, const task_sptr_t &task);
		
		/** \brief Добавить задачу в очередь
		 * \param task Задача.
		 * \return Возвращает объект задачи.
		 */
		task_wptr_t enqueue(const task_sptr_t &task);
		
		/** \brief Обработать задачу из очереди.
		 * NOTE Объект ссылки может быть обнулён в процессе выполнения.
		 * Если задача повторяющаяся, её время после выполнения будет переустановлено.
		 * \param task Задача.
		 */
		void process_task(task_sptr_t &task);
		
		/** \brief Выполнить задачу, если её выполнение разрешено.
		 * NOTE Объект ссылки может быть обнулён в процессе выполнения.
		 * \param task Задача.
		 */
		static void execute_task(task_sptr_t &task);
	};
	
}

#endif // UTILS_TASK_QUEUE_HPP
