#include <freetype/FTLibrary.hpp>
#include <freetype/macro.hpp>
#include <utils/log.hpp>


namespace freetype
{
	
	FTLibraryWPtr FTLibrary::m_instance;
	
	
	FTLibrary::FTLibrary()
	: m_library(NULL)
	{
		FTCHECK( FT_Init_FreeType(&m_library) );
	}
	
	
	FTLibrary::~FTLibrary()
	{
		FT_Done_FreeType(m_library);
	}
	
	
	FTLibrarySPtr FTLibrary::instance()
	{
		FTLibrarySPtr result = m_instance.lock();
		if (result) {
			return result;
		}
		result.reset(new FTLibrary);
		m_instance = result;
		return result;
	}
	
}
