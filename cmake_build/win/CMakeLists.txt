cmake_minimum_required(VERSION 2.8)
project(eos_client)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Project dir path
set(PROJECT_DIR ${CMAKE_SOURCE_DIR}/../../)
# Windows project path
set(WINDOWS_DIR ${PROJECT_DIR}/platform/win)

# Point to our own cmake modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_DIR}/cmake_modules)

file(GLOB SRC_PLATFORM ${PROJECT_DIR}/platform/win/*.h ${PROJECT_DIR}/platform/win/*.c ${PROJECT_DIR}/platform/win/*.hpp ${PROJECT_DIR}/platform/win/*.cpp)
file(GLOB SRC_GAME ${PROJECT_DIR}/game/*.hpp ${PROJECT_DIR}/game/*.cpp)
file(GLOB SRC_GAME_GRAPHICS ${PROJECT_DIR}/game/graphics/*.hpp ${PROJECT_DIR}/game/graphics/*.cpp)
file(GLOB SRC_GAME_UTILS ${PROJECT_DIR}/game/utils/*.hpp ${PROJECT_DIR}/game/utils/*.cpp)

set(SOURCE_FILES
	${SRC_PLATFORM}
	${SRC_GAME}
	${SRC_GAME_GRAPHICS}
	${SRC_GAME_UTILS}
)

source_group("platform" FILES ${SRC_PLATFORM})
source_group("game" FILES ${SRC_GAME})
source_group("game\\graphics" FILES ${SRC_GAME_GRAPHICS})
source_group("game\\utils" FILES ${SRC_GAME_UTILS})

# Find OpenGL
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
	include_directories(${OPENGL_INCLUDE_DIR})
else (OPENGL_FOUND)
	message("OpenGL environment missing")
endif (OPENGL_FOUND)
# Find OpenGL extension wrangler
find_library(GLEW_LIBRARIES glew32
	PATHS ${PROJECT_DIR}/lib/win/${LIB_PLATFORM}
	DOC "OpenGL libraries"
)

# Find SDL2
find_library(SDL_LIB SDL2
	PATHS ${PROJECT_DIR}/lib/win/${LIB_PLATFORM}
	DOC "SDL libraries"
)
# Find SDL2 Mixer
find_library(SDLMIXER_LIB SDL2_Mixer
	PATHS ${PROJECT_DIR}/lib/win/${LIB_PLATFORM}
	DOC "SDL libraries"
)
set(SDL_LIBRARIES
	${SDL_LIB}
	${SDLMIXER_LIB}
)


include_directories(
	${WINDOWS_DIR}
	${PROJECT_DIR}
	${PROJECT_DIR}/game
	${PROJECT_DIR}/include/win
)

set(EXT_LIBRARIES
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	${SDL_LIBRARIES}
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
