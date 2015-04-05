#ifndef UTILS_STRBUILDER_HPP
#define	UTILS_STRBUILDER_HPP

#include <sstream>


namespace utils
{
	
	class strbuilder
	{
	public:
		strbuilder() : m_oss(new ostringstream) {}
		
		inline std::string str() const { return m_oss->str(); }
		
		inline operator std::string() const { return str(); }
		
		inline strbuilder& reset() { m_oss.reset(new ostringstream); return *this; }
		
	private:
		struct ostringstream : public std::ostringstream {};
		std::shared_ptr<ostringstream> m_oss;
		
		template <typename T> friend inline const strbuilder& operator <<(const strbuilder &lhs, const T &rhs)
		{
			*lhs.m_oss << rhs; return lhs;
		}
	};
	
	
	inline std::string to_string(const strbuilder &sb)
	{
		return sb.str();
	}
	
}

#endif // UTILS_STRBUILDER_HPP
