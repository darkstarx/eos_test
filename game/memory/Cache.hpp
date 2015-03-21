#ifndef MEMORY_CACHE_HPP
#define MEMORY_CACHE_HPP

#include <graphics/forwards.hpp>
#include <utils/log.hpp>
#include <utils/forwards.hpp>
#include <map>


namespace memory
{
	
	/** \brief Кэш объектов
	 */
	class Cache
	{
	private:
		template <class> struct CleanerS;
		template <class> struct CleanerW;
		
		struct HolderBase
		{
			HolderBase();
			time_t load_time() const;
			time_t access_time() const;
			void touch();
		private:
			time_t m_load_time;
			time_t m_access_time;
		};
		
		/** \brief Обертка над объектом, хранящемся в кэше
		 * Класс предназначен для хранения указателя на объект. Удерживает объект в памяти.
		 */
		template <class TSPtr>
		struct HolderS : public HolderBase
		{
		public:
			typedef CleanerS<TSPtr> CleanerT;
			explicit HolderS(const TSPtr& org);
			inline bool expired() const { return false; }
			int use_count() const;
			TSPtr& value();
			const TSPtr& value() const;
		private:
			TSPtr m_value;
		};
		
		/** \brief Обертка над объектом, хранящемся в кэше
		 * Класс предназначен для хранения указателя на объект. Не удерживает объект в памяти.
		 */
		template <class TSPtr>
		struct HolderW : public HolderBase
		{
		public:
			typedef CleanerW<TSPtr> CleanerT;
			typedef std::weak_ptr<typename TSPtr::element_type> T;
			HolderW();
			explicit HolderW(const TSPtr& org);
			inline bool expired() const { return m_value.expired(); }
			int use_count() const;
			TSPtr value() const;
		private:
			T m_value;
		};
		
		typedef std::map<std::string /*key*/, HolderW<utils::bytearray_sptr_t> /*resource*/> bytearray_cache_t;
		typedef std::map<std::string /*key*/, HolderS<graphics::TextureSPtr> /*resource*/> texture_cache_t;
		
	public:
		Cache(const Cache& root) = delete;
		
		Cache& operator=(const Cache&) = delete;
		
		/** \brief Создать кэш
		 */
		static void create();
		
		/** \brief Разрушить кэш
		 */
		static void destroy();
		
		/** \brief Проверить, что кэш создан
		 */
		static bool is_alive() { return m_instance != 0; }
		
		/** \brief Получить экземпляр кэша
		 */
		static Cache& instance();
		
		/** \brief Поместить объект в кэш
		 * Если объект уже находится в кэше, то продляется время его хранения в этом кэше.
		 * \param key Ключ объекта, по которому он будет идентифицироваться в кэше.
		 * \param obj Умный указатель на объект, который следует поместить в кэш.
		 * \tparam TSPtr Тип объекта, который следует поместить в кэш.
		 */
		template <class TSPtr>
		void cache_obj(const std::string& key, const TSPtr& obj);
		
		/** \brief Получить закэшированный объект
		 * Если объект не найден в кэше, то возвращается пустой указатель.
		 * \param key Ключ объекта в кэше.
		 * \return Указатель на объект (пустой, если объект не найден).
		 */
		template <class TSPtr>
		TSPtr get_obj(const std::string& key);
		
		/** \brief Выполнить очистку кэша от объекта с указанным ключом
		 * Удаляет объект указанного типа с указанным ключом из соответствующего кэша. Если тип не указан, то
		 * производится очистка всех кэшей от объекта с указанным ключом.
		 * \param key Ключ объекта в кэше.
		 * \tparam TSptr Тип объекта, который следует удалить из кэша.
		 */
		template <class TSPtr>
		void clean(const std::string& key);
		
		/** \brief Выполнить очистку кэшей
		 * Удаляет из всех кэшей объекты с истекшим сроком жизни в кэше (Deprecated).
		 * Удаляет из всех кэшей объекты, которые на момент очистки не используются (Actual).
		 */
		void clean();
		
		/** \brief Выполнить принудительную очистку кэшей
		 * Принудительно удаляет из всех кэшей объекты, которые на момент очистки не используются.
		 */
		void force_clean();
		
	private:
		static Cache* m_instance;			///< Единственный экземпляр отрисовщика
		texture_cache_t m_textures;			///< Кэш текстур
		bytearray_cache_t m_bytearrays;		///< Кэш наборов данных
		
		Cache();
		
		~Cache();
		
		/** \brief Выполнить очистку кэша
		 * Удаляет из кэша объекты, которые на текущий момент больше никем не используются.
		 * \param cache Кэш, который следует очистить.
		 * \tparam HolderT Тип обертки над закэшированным объектом (может быть как \c HolderS так и \c HolderW).
		 */
		template <class HolderT>
		static void do_clean(std::map<std::string, HolderT>& cache);
		
		/** \brief Выполнить принудительную очистку кэша
		 * В текущей реализации чистильщиков \c CleanerS и \c CleanerW ничем не отличается от обычной очистки кэша.
		 * \param cache Кэш, который следует очистить.
		 * \tparam HolderT Тип обертки над закэшированным объектом (может быть как \c HolderS так и \c HolderW).
		 */
		template <class HolderT>
		static void do_force_clean(std::map<std::string, HolderT>& cache);
		
		
		/** \brief Найти объект в кэше
		 * Выполняет поиск в кэше удерживаемых (shared_ptr) объектов.
		 * \param key Ключ, по которому объект идентифицируется в кэше.
		 * \param cache Кэш, в котором требуется произвести поиск объекта.
		 * \tparam T Тип умного указателя на объект (shared_ptr< object_type >).
		 * \return Итератор, указывающий на найденный элемент в кэше или на конец кэша, если объект не найден.
		 */
		template <class T>
		static typename std::map<std::string, HolderS<T> >::iterator cached_resource(const std::string& key, std::map<std::string, HolderS<T> >& cache);
		
		/** \brief Найти объект в кэше
		 * Производится поиск в кэше не удерживаемых (weak_ptr) объектов.
		 * \param key Ключ, по которому объект идентифицируется в кэше.
		 * \param cache Кэш, в котором требуется произвести поиск объекта.
		 * \tparam T Тип умного указателя на объект (shared_ptr< object_type >).
		 * \return Итератор, указывающий на найденный элемент в кэше или на конец кэша, если объект не найден.
		 */
		template <class T>
		static typename std::map<std::string, HolderW<T> >::iterator cached_resource(const std::string& key, std::map<std::string, HolderW<T> >& cache);
		
		/** \brief Сохранить объект в кэше
		 * Выполняет сохранение объекта в кэше, объект не разрушается до тех пор, пока находится в кэше. Если объект уже находится в кэше, то продлевается
		 *  время его хранения в этом кэше.
		 * \param key Ключ, по которому объект идентифицируется в кэше.
		 * \param v Объект, который должен быть сохранен в кэше.
		 * \param cache Кэш, в котором должен быть сохранен объект.
		 * \tparam T Тип умного указателя на объект (shared_ptr< object_type >).
		 * \return Обертка над объектом, в которой он был сохранен в кэше.
		 */
		template <class T>
		static const HolderS<T>& cache_resource(const std::string& key, const T& v, std::map<std::string, HolderS<T> >& cache);
		
		/** \brief Сохранить объект в кэше
		 * Выполняет сохранение объекта в кэше, объект может быть разрушен до того, как покинет кэш. Если объект уже находится в кэше, то продлевается
		 *  время его хранения в этом кэше.
		 * \param key Ключ, по которому объект идентифицируется в кэше.
		 * \param v Объект, который должен быть сохранен в кэше.
		 * \param cache Кэш, в котором должен быть сохранен объект.
		 * \tparam T Тип умного указателя на объект (shared_ptr< object_type >).
		 * \return Обертка над объектом, в которой он был сохранен в кэше.
		 */
		template <class T>
		static const HolderW<T>& cache_resource(const std::string& key, const T& v, std::map<std::string, HolderW<T> >& cache);
	};
	
}

inline memory::Cache& cache() { return memory::Cache::instance(); }

#endif // MEMORY_CACHE_HPP
