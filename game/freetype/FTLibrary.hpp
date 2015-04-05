#ifndef FREETYPE_FTLIBRARY_HPP
#define FREETYPE_FTLIBRARY_HPP

#include <freetype/types.hpp>
#include <freetype/forwards.hpp>
#include <ft2build.h>
#include <freetype.h>


namespace freetype
{
	
	/** \brief Класс, реализующий обёртку над FT_Library
	 * Реализует singleton.
	 * Функция instance() возвращает sptr на созданный объект, его нужно сохранить в месте, где объект FTLibrary может
	 * быть использован, и разрушать, когда он уже не нужен.
	 */
	class FTLibrary
	{
	private:
		FTLibrary();
	
	public:
		~FTLibrary();
		
		static FTLibrarySPtr instance();
		
		inline const FT_Library& get() const { return m_library; }
		
	private:
		static FTLibraryWPtr m_instance;	///< Слабый указатель на экземпляр
		FT_Library m_library;				///< Библиотека freetype
	};
	
}

#endif // FREETYPE_FTLIBRARY_HPP
