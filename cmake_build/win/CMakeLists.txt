cmake_minimum_required(VERSION 2.8)
project(eos_client)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(PROJECT_DIR ${CMAKE_SOURCE_DIR}/../../)

file(GLOB SRC_PLATFORM ${PROJECT_DIR}/platform/win/*.h ${PROJECT_DIR}/platform/win/*.cpp)
file(GLOB SRC_PLATFORM_GAME ${PROJECT_DIR}/platform/win/game/*.h ${PROJECT_DIR}/platform/win/game/*.cpp)
file(GLOB SRC_GAME ${PROJECT_DIR}/game/*.h ${PROJECT_DIR}/game/*.cpp)
file(GLOB SRC_GAME_GRAPHICS ${PROJECT_DIR}/game/graphics/*.h ${PROJECT_DIR}/game/graphics/*.cpp)
file(GLOB SRC_GAME_UTILS ${PROJECT_DIR}/game/utils/*.h ${PROJECT_DIR}/game/utils/*.cpp)

set(SOURCE_FILES
	${SRC_PLATFORM}
	${SRC_PLATFORM_GAME}
	${SRC_GAME}
	${SRC_GAME_GRAPHICS}
	${SRC_GAME_UTILS}
)

source_group("platform" FILES ${SRC_PLATFORM})
source_group("platform\\game" FILES ${SRC_PLATFORM_GAME})
source_group("game" FILES ${SRC_GAME})
source_group("game\\graphics" FILES ${SRC_GAME_GRAPHICS})
source_group("game\\utils" FILES ${SRC_GAME_UTILS})

find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
	include_directories(${OPENGL_INCLUDE_DIR})
	set(OGL_LIB ${OPENGL_LIBRARIES})
else (OPENGL_FOUND)
	message("OpenGL environment missing")
endif (OPENGL_FOUND)

find_library(GLEW_LIB glew32
	PATHS ${PROJECT_DIR}/lib/win/${LIB_PLATFORM}
	DOC "OpenGL libraries"
)

find_library(SDL_LIB SDL2
	PATHS ${PROJECT_DIR}/lib/win/${LIB_PLATFORM}
	DOC "SDL libraries"
)

find_library(SDLMIXER_LIB SDL2_Mixer
	PATHS ${PROJECT_DIR}/lib/win/${LIB_PLATFORM}
	DOC "SDL libraries"
)

find_library(GLOG_LIB libglog
	PATHS ${PROJECT_DIR}/lib/win/${LIB_PLATFORM}
	DOC "Logging library"
)


include_directories(
	${PROJECT_DIR}
	${PROJECT_DIR}/game
	${PROJECT_DIR}/game/graphics
	${PROJECT_DIR}/game/utils
	${PROJECT_DIR}/include
	${PROJECT_DIR}/lib/glog/src/windows
)

set(EXT_LIBRARIES
	${OGL_LIB}
	${GLEW_LIB}
	${SDL_LIB}
	${SDLMIXER_LIB}
	${GLOG_LIB}
)

include(${PROJECT_DIR}/cmake_build/common/game.cmake)

list(APPEND GAME_DEFINES
	__DESKTOP__
)

add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${EXT_LIBRARIES})

set_target_properties(${PROJECT_NAME}
	PROPERTIES
	COMPILE_DEFINITIONS "${GAME_DEFINES}"
)
