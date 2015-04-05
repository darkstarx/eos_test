#include <utils/string_utils.hpp>
#include <string.h>
#include <wctype.h>
#include <algorithm>
#include <utf8.h>


namespace utils
{
	
	uint_array string_utils::string_to_codearray(std::string const &text)
	{
		uint_array result;
		for (std::string::const_iterator it = text.begin(); it != text.end(); ) {
			result.push_back(utf8::next(it, text.end()));
		}
		return result;
	}
	
	
	std::string string_utils::codearray_to_string(uint_array const &text)
	{
		std::string result;
		for (uint_array::const_iterator it = text.begin(); it != text.end(); it++) {
			utf8::append(*it, std::back_inserter(result));
		}
		return result;
	}
	
	
	std::string string_utils::replace_str(std::string text, const std::string &from, const std::string &to)
	{
		for (std::string::size_type index = 0; index = text.find(from, index), index != std::string::npos; ) {
			text.replace(index, from.length(), to);
			index += to.length();
		}
		return text;
	}
	
	
	std::string string_utils::replace_first(std::string text, const std::string &from, const std::string &to, bool &found)
	{
		found = false;
		std::string::size_type index = text.find(from, 0);
		if (index != std::string::npos) {
			text.replace(index, from.length(), to);
			found = true;
		}
		return text;
	}
	
	
	void string_utils::trim_left(std::string &text)
	{
		std::string::iterator str_it = text.begin();
		const std::string::iterator str_end = text.end();
		for (; str_it != str_end; ++str_it ) if( (*str_it) != ' ') break;
		text.erase(text.begin(), str_it);
	}
	
	
	std::string string_utils::trim_all(const std::string &text)
	{
		if (!text.size()) return "";
		
		std::string::const_iterator begin, end;
		
		// Ищем начало строки (не пробел)
		for (begin = text.begin(); begin != text.end(); ++begin) {
			if (!std::isspace(*begin)) break;
		}
		
		// Если строка из одних пробелов, возвращаем пустую строку
		if (begin == text.end()) return "";
		
		// Ищем конец строки (не пробел) (end()-1 чтобы итератор указывал на символ)
		for (end = text.end() - 1; end != text.begin(); --end) {
			if (!std::isspace(*end)) break;
		}
		
		// Инициализация через указатели (end+1 - итератор на конец символа)
		std::string result(begin, end + 1);
		return result;
	}
	
	
	void string_utils::replace_nl(std::string &text)
	{
		std::replace(text.begin(), text.end(), '\n', ' ');
	}
	
	
	std::string string_utils::replace_chars(std::string text, const std::string &from, const std::string &to)
	{
		uint_array w_text = string_to_codearray(text);
		uint_array w_from = string_to_codearray(from);
		uint_array w_to = string_to_codearray(to);
		size_t sz_f = w_from.size();
		size_t sz_t = w_to.size();
		size_t cnt = sz_f < sz_t ? sz_f : sz_t;
		if (cnt < 1) return text;
		
		for (size_t i = 0; i < cnt; i++) {
			for(uint_array::iterator it = w_text.begin(); it != w_text.end(); it++) {
				if (*it == w_from[i]) {
					*it = w_to[i];
				}
			}
		}
		std::string result = codearray_to_string(w_text);
		return result;
	}
	
	
	uint32_t string_utils::get_number_of_unicode_chars_in_text(const std::string &utf8_text)
	{
		const char* str_begin = utf8_text.c_str();
		const char* str_end = str_begin + utf8_text.size();
		const char* char_current = str_begin;
		uint32_t result = 0;
		
		while(char_current != str_end) {
			utf8::next(char_current,str_end);
			++result;
		}
		return result;
	}
	
	
	std::string string_utils::remove_text_between_chars(const std::string &source, const char c1, const char c2)
	{
		std::string result;
		
		// Текущая позиция в исходной строке, с которой ведём поиск при итерации цикла
		std::string::size_type pos = 0;
		
		while (true) {
			// Ищем первый символ от текущей позиции pos
			std::string::size_type pos1 = source.find('<', pos);
			if (pos1 != std::string::npos) {
				// Добавляем к результату строку от текущей позиции до найденного первого символа
				result += source.substr(pos, pos1 - pos);
			} else {
				// Если первый символ не найден, то до конца исходной строки, и выходим из цикла
				result += source.substr(pos);
				break;
			}
			// Ищем второй символ, от найденной позиции pos1
			pos1 = source.find('>', pos1);
			// Если второй символ не найден, выходим из цикла, первый символ и всё что шло после него, не попадут в результат
			if (pos1 == std::string::npos) break;
			// Если второй символ найден на конце строки, выходим из цикла
			if (pos1 == source.size() - 1) break;
			// Иначе помещаем текущую позицию после второго символа и повторяем итерацию
			pos = pos1 + 1;
		}
		return result;
	}
	
	
	bool string_utils::is_control(uint32_t symbol)
	{
		return (symbol < 32) || (symbol >= 127 && symbol < 160);
	}
	
}
