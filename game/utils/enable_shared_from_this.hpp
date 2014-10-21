#ifndef ENABLE_SHARED_FROM_THIS_HPP
#define ENABLE_SHARED_FROM_THIS_HPP
#include <memory>


namespace utils
{
	
	class enable_shared_from_this : public std::enable_shared_from_this<enable_shared_from_this>
	{
	public:
		template <class T>
		inline std::shared_ptr<T> shared_from_this()
		{
			return std::static_pointer_cast<T>(
				std::enable_shared_from_this<enable_shared_from_this>::shared_from_this()
			);
		}
		
		template <class T>
		inline std::shared_ptr<const T> shared_from_this() const
		{
			return std::static_pointer_cast<const T>(
				std::enable_shared_from_this<enable_shared_from_this>::shared_from_this()
			);
		}
		
		template <class T>
		inline std::weak_ptr<T> weak_from_this()
		{
			return std::weak_ptr<T>(shared_from_this<T>());
		}
		
		template <class T>
		inline std::weak_ptr<const T> weak_from_this() const
		{
			return std::weak_ptr<const T>(shared_from_this<const T>());
		}
	};
	
}


using utils::enable_shared_from_this;

#endif//ENABLE_SHARED_FROM_THIS_HPP
