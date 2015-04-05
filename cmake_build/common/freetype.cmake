set(FREETYPE_DIR "${PROJECT_DIR}/lib/freetype")
list(APPEND FREETYPE_INCLUDES
  ${FREETYPE_DIR}/include
  ${FREETYPE_DIR}/include/config
)
list(APPEND FREETYPE_DEFINES
  DARWIN_NO_CARBON=1
  FT2_BUILD_LIBRARY=1
  FT_CONFIG_CONFIG_H=<ftconfig.h>
)
list(APPEND FREETYPE_SOURCES
  ${FREETYPE_DIR}/src/autofit/autofit.c
  ${FREETYPE_DIR}/src/base/ftadvanc.c
  ${FREETYPE_DIR}/src/base/ftbbox.c
  ${FREETYPE_DIR}/src/base/ftbdf.c
  ${FREETYPE_DIR}/src/base/ftbitmap.c
  ${FREETYPE_DIR}/src/base/ftcalc.c
  ${FREETYPE_DIR}/src/base/ftcid.c
  ${FREETYPE_DIR}/src/base/ftdbgmem.c
  ${FREETYPE_DIR}/src/base/ftdebug.c
  ${FREETYPE_DIR}/src/base/ftfstype.c
  ${FREETYPE_DIR}/src/base/ftgasp.c
  ${FREETYPE_DIR}/src/base/ftgloadr.c
  ${FREETYPE_DIR}/src/base/ftglyph.c
  ${FREETYPE_DIR}/src/base/ftgxval.c
  ${FREETYPE_DIR}/src/base/ftinit.c
  ${FREETYPE_DIR}/src/base/ftlcdfil.c
  ${FREETYPE_DIR}/src/base/ftmm.c
  ${FREETYPE_DIR}/src/base/ftobjs.c
  ${FREETYPE_DIR}/src/base/ftotval.c
  ${FREETYPE_DIR}/src/base/ftoutln.c
  ${FREETYPE_DIR}/src/base/ftpatent.c
  ${FREETYPE_DIR}/src/base/ftpfr.c
  ${FREETYPE_DIR}/src/base/ftrfork.c
  ${FREETYPE_DIR}/src/base/ftsnames.c
  ${FREETYPE_DIR}/src/base/ftstream.c
  ${FREETYPE_DIR}/src/base/ftstroke.c
  ${FREETYPE_DIR}/src/base/ftsynth.c
  ${FREETYPE_DIR}/src/base/ftsystem.c
  ${FREETYPE_DIR}/src/base/fttrigon.c
  ${FREETYPE_DIR}/src/base/fttype1.c
  ${FREETYPE_DIR}/src/base/ftutil.c
  ${FREETYPE_DIR}/src/base/ftwinfnt.c
  ${FREETYPE_DIR}/src/base/ftxf86.c
  ${FREETYPE_DIR}/src/bdf/bdf.c
  ${FREETYPE_DIR}/src/bzip2/ftbzip2.c
  ${FREETYPE_DIR}/src/cache/ftcache.c
  ${FREETYPE_DIR}/src/cff/cff.c
  ${FREETYPE_DIR}/src/cid/type1cid.c
  ${FREETYPE_DIR}/src/gzip/ftgzip.c
  ${FREETYPE_DIR}/src/lzw/ftlzw.c
  ${FREETYPE_DIR}/src/pcf/pcf.c
  ${FREETYPE_DIR}/src/pfr/pfr.c
  ${FREETYPE_DIR}/src/psaux/psaux.c
  ${FREETYPE_DIR}/src/pshinter/pshinter.c
  ${FREETYPE_DIR}/src/psnames/psmodule.c
  ${FREETYPE_DIR}/src/raster/raster.c
  ${FREETYPE_DIR}/src/sfnt/sfnt.c
  ${FREETYPE_DIR}/src/smooth/smooth.c
  ${FREETYPE_DIR}/src/truetype/truetype.c
  ${FREETYPE_DIR}/src/type1/type1.c
  ${FREETYPE_DIR}/src/type42/type42.c
  ${FREETYPE_DIR}/src/winfonts/winfnt.c
)
file(GLOB FREETYPE_PUBLIC_HEADERS "include/*.h")
file(GLOB FREETYPE_PUBLIC_CONFIG_HEADERS "include/config/*.h")
file(GLOB FREETYPE_PRIVATE_HEADERS "include/internal/*.h")

include_directories(${FREETYPE_INCLUDES})

add_library(freetype STATIC
  ${FREETYPE_PUBLIC_HEADERS}
  ${FREETYPE_PUBLIC_CONFIG_HEADERS}
  ${FREETYPE_PRIVATE_HEADERS}
  ${FREETYPE_SOURCES}
)
set_target_properties(freetype
  PROPERTIES
  COMPILE_DEFINITIONS "${FREETYPE_DEFINES}"
)
