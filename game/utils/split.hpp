#ifndef UTILS_SPLIT_HPP
#define UTILS_SPLIT_HPP

#include <string>
#include <set>


namespace utils
{
	
	/** \brief Разделить строку на подстроки.
	 * \tparam Seq Класс выходной последовательности.
	 * \param out Результат разделения.
	 * \param in Входная строка.
	 * \param ch Символ разделителя. */
	template <class Seq> void split(Seq& out, const std::string& in, const char ch)
	{
		std::string::size_type pos, last_pos = 0;
		while ((pos = in.find(ch, last_pos)) != std::string::npos) {
			out.push_back(in.substr(last_pos, pos - last_pos));
			last_pos = pos + 1;
		}
		if (last_pos < in.length()) out.push_back(in.substr(last_pos));
		else if (in.length() && in[in.length()-1]==ch) out.push_back(std::string(""));
	}
	
	/** \brief Разделить строку на подстроки.
	 * Конкретизация для std::set (пропускает пустые строки).
	 */
	template<> void split(std::set<std::string>& out, const std::string& in, const char ch);
	
}


#endif // UTILS_SPLIT_HPP

