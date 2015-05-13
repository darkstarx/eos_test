#ifndef UTILS_STRING_UTILS_HPP
#define UTILS_STRING_UTILS_HPP

#include <string>
#include <vector>
#include <stdint.h>


namespace utils
{
	
	typedef std::vector<uint32_t> uint_array;
	
	class string_utils
	{
	private:
		static uint_array string_to_codearray(std::string const &text);
		static std::string codearray_to_string(uint_array const &text);
		
	public:
		/** \brief Заменить все вхождения подстроки в строке
		 * \param text Текст для изменения.
		 * \param from Подстрока для поиска.
		 * \param to Подстрока для замены найденного.
		 * \return Измененная строка.
		 */
		static std::string replace_str(std::string text, const std::string &from, const std::string &to);

		/** \brief Заменить первое вхождение подстроки в строке
		 * \param text Текст для изменения.
		 * \param from Подстрока для поиска.
		 * \param to Подстрока для замены найденного.
		 * \param found Функция возвращает, была ли замена.
		 * \return Измененная строка.
		 */
		static std::string replace_first(std::string text, const std::string &from, const std::string &to, bool &found);
		
		/** \brief Удалить все символы пробела из начала строки
		 * \param text Исходный текст.
		 * \return Текст без пробелов в начале.
		 */
		static void trim_left(std::string &text);
		
		/** \brief Удалить все пробелы с начала и конца строки
		 * \param text Исходный текст.
		 * \return Текст без пробелов по краям.
		 */
		static std::string trim_all(const std::string& text);
		
		/** \brief Заменить все '\n'  на ' ' в строке
		 * \param src_string текст для изменения
		 */
		static void replace_nl(std::string &src_string);
		
		/** \brief Выполнить посимвольную замену.
		 * Заменяет i-й символ из \c from на i-й символ из \c to. Длина строки \c from должна быть равна длине
		 * строки \c to. Количество байт на символ не имеет значения.
		 * \param text Текст для замены.
		 * \param from Строка-набор заменяемых символов.
		 * \param to Строка-набор заменяющих символов.
		 * \return Измененная строка.
		 */
		static std::string replace_chars(std::string text, const std::string &from, const std::string &to);
		
		/** \brief Получить количество юникод символов в тексте
		 * \param utf8_text Текст.
		 * \return Количество символов.
		 */
		static uint32_t get_number_of_unicode_chars_in_text(const std::string &utf8_text);
		
		/** \brief Удалить символы из строки
		 * Выполняет удаление из исходной строки \c source текста между символами \c c1 и \c с2, включая сами
		 * символы. Если в тексте есть символ \c c1, но после него отсутствует символ \c c2, то все оставшиеся
		 * символы после \c c1 будут удалены.
		 * \return Строка с удалённым текстом.
		 */
		static std::string remove_text_between_chars(const std::string &source, const char c1, const char c2);
		
		/** \brief Определить, является ли символ управляющим
		 * Управляющими символами считаются все в таблице Unicode до пробела (&#32), и с символа delete (&#127) до символа
		 * Неразрывный пробел (#&160), не включая последний. Мягкий перенос и прочие символы, не имеющие глифов, считаются
		 * обычными невидимыми символами, которые должны игнорироваться.
		 * \param symbol Код символа в кодировке Unicode.
		 */
		static bool is_control(uint32_t symbol);
	};
	
}

#endif // UTILS_STRING_UTILS_HPP
