#ifndef FREETYPE_FORWARDS_HPP
#define FREETYPE_FORWARDS_HPP

#include <memory>


namespace freetype
{
	
	class FTLibrary;
	typedef std::shared_ptr<FTLibrary> FTLibrarySPtr;
	typedef std::weak_ptr<FTLibrary> FTLibraryWPtr;
	
	class FTFace;
	typedef std::shared_ptr<FTFace> FTFaceSPtr;
	typedef std::weak_ptr<FTFace> FTFaceWPtr;
	
	class FTGlyph;
	typedef std::shared_ptr<FTGlyph> FTGlyphSPtr;
	typedef std::weak_ptr<FTGlyph> FTGlyphWPtr;
	
}

#endif // FREETYPE_FORWARDS_HPP
