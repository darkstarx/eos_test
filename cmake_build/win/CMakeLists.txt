cmake_minimum_required(VERSION 2.8)
project(eos_client)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Project dir path
set(PROJECT_DIR ${CMAKE_SOURCE_DIR}/../..)
# Windows project path
set(WINDOWS_DIR ${PROJECT_DIR}/platform/win)
# Common desktop platform files directory
set(DESKTOP_DIR ${PROJECT_DIR}/platform/desktop)

# Point to our own cmake modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_DIR}/cmake_modules)

file(GLOB SRC_PLATFORM ${PROJECT_DIR}/platform/win/*.h ${PROJECT_DIR}/platform/win/*.c ${PROJECT_DIR}/platform/win/*.hpp ${PROJECT_DIR}/platform/win/*.cpp)
file(GLOB SRC_DESKTOP ${PROJECT_DIR}/platform/desktop/*.hpp ${PROJECT_DIR}/platform/desktop/*.cpp)
file(GLOB SRC_GAME ${PROJECT_DIR}/game/*.hpp ${PROJECT_DIR}/game/*.cpp)
file(GLOB SRC_GAME_GRAPHICS ${PROJECT_DIR}/game/graphics/*.hpp ${PROJECT_DIR}/game/graphics/*.cpp)
file(GLOB SRC_GAME_RESOURCES ${PROJECT_DIR}/game/resources/*.hpp ${PROJECT_DIR}/game/resources/*.cpp)
file(GLOB SRC_GAME_UTILS ${PROJECT_DIR}/game/utils/*.hpp ${PROJECT_DIR}/game/utils/*.cpp)
file(GLOB SRC_GAME_MEMORY ${PROJECT_DIR}/game/memory/*.hpp ${PROJECT_DIR}/game/memory/*.cpp)

set(SOURCE_FILES
	${SRC_PLATFORM}
	${SRC_DESKTOP}
	${SRC_GAME}
	${SRC_GAME_GRAPHICS}
	${SRC_GAME_RESOURCES}
	${SRC_GAME_UTILS}
	${SRC_GAME_MEMORY}
)

source_group("platform" FILES ${SRC_PLATFORM} ${SRC_DESKTOP})
source_group("game" FILES ${SRC_GAME})
source_group("game\\graphics" FILES ${SRC_GAME_GRAPHICS})
source_group("game\\resources" FILES ${SRC_GAME_RESOURCES})
source_group("game\\utils" FILES ${SRC_GAME_UTILS})
source_group("game\\memory" FILES ${SRC_GAME_MEMORY})

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
	${DESKTOP_DIR}
	${PROJECT_DIR}
	${PROJECT_DIR}/game
	${PROJECT_DIR}/include/win
	${PROJECT_DIR}/lib/glm
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

# Copy built binaries and resources for them into the `bin` directory
string(REPLACE "/" "\\" PROJECT_DIR_WIN ${PROJECT_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND xcopy ${PROJECT_DIR_WIN}\\resources\\* $(ProjectDir)resources\\ /e/q/r/y)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND xcopy ${PROJECT_DIR_WIN}\\assets\\* $(ProjectDir)assets\\ /e/q/r/y)

# Resources and assets paths
list(APPEND GAME_DEFINES
	RESOURCES_PATH="resources\\\\"
	ASSETS_PATH="assets\\\\"
)
# Set c++ definitions
set_target_properties(${PROJECT_NAME}
	PROPERTIES
	COMPILE_DEFINITIONS "${GAME_DEFINES}"
)
