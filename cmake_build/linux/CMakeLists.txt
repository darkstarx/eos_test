cmake_minimum_required(VERSION 2.8)
project(eos_client)

# Project dir path
set(PROJECT_DIR ${CMAKE_SOURCE_DIR}/../..)
# Linux project path
set(LINUX_DIR ${PROJECT_DIR}/platform/linux)
# Common desktop platform files directory
set(DESKTOP_DIR ${PROJECT_DIR}/platform/desktop)

# Point to our own cmake modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_DIR}/cmake_modules)

file(GLOB SRC_PLATFORM ${PROJECT_DIR}/platform/linux/*.hpp ${PROJECT_DIR}/platform/linux/*.cpp)
file(GLOB SRC_DESKTOP ${PROJECT_DIR}/platform/desktop/*.hpp ${PROJECT_DIR}/platform/desktop/*.cpp)
file(GLOB SRC_GAME ${PROJECT_DIR}/game/*.hpp ${PROJECT_DIR}/game/*.cpp)
file(GLOB SRC_GAME_GRAPHICS ${PROJECT_DIR}/game/graphics/*.hpp ${PROJECT_DIR}/game/graphics/*.cpp)
file(GLOB SRC_GAME_RESOURCES ${PROJECT_DIR}/game/resources/*.hpp ${PROJECT_DIR}/game/resources/*.cpp)
file(GLOB SRC_GAME_UTILS ${PROJECT_DIR}/game/utils/*.hpp ${PROJECT_DIR}/game/utils/*.cpp)
file(GLOB SRC_GAME_MEMORY ${PROJECT_DIR}/game/memory/*.hpp ${PROJECT_DIR}/game/memory/*.cpp)
file(GLOB SRC_GAME_FREETYPE ${PROJECT_DIR}/game/freetype/*.hpp ${PROJECT_DIR}/game/freetype/*.cpp)

set(SOURCE_FILES
	${SRC_PLATFORM}
	${SRC_DESKTOP}
	${SRC_GAME}
	${SRC_GAME_GRAPHICS}
	${SRC_GAME_RESOURCES}
	${SRC_GAME_UTILS}
	${SRC_GAME_MEMORY}
	${SRC_GAME_FREETYPE}
)

# Enable CCache
find_program(CCACHE_PATH ccache)
if(NOT ${CCACHE_PATH} MATCHES CCACHE_PATH-NOTFOUND)
	message(STATUS "CCache found: ${CCACHE_PATH}")
	set(CMAKE_C_COMPILER_ARG1 ${CMAKE_C_COMPILER})
	set(CMAKE_C_COMPILER ${CCACHE_PATH})
	set(CMAKE_CXX_COMPILER_ARG1 ${CMAKE_CXX_COMPILER})
	set(CMAKE_CXX_COMPILER ${CCACHE_PATH}) 
else()
	message(STATUS "CCache not found")
endif()

# Find OpenGL
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
	include_directories(${OPENGL_INCLUDE_DIR})
else (OPENGL_FOUND)
	message("OpenGL environment missing")
endif (OPENGL_FOUND)

# Find SDL2
find_package(SDL2 REQUIRED)
if (SDL2_FOUND)
	include_directories(${SDL2_INCLUDE_DIR})
else (SDL2_FOUND)
	message("SDL2 environment missing")
endif (SDL2_FOUND)


include_directories(
	${LINUX_DIR}
	${DESKTOP_DIR}
	${PROJECT_DIR}/game
	${PROJECT_DIR}/lib/glm
)

include(${PROJECT_DIR}/cmake_build/common/freetype.cmake)
include(${PROJECT_DIR}/cmake_build/common/game.cmake)


set(EXT_LIBRARIES
	${OPENGL_LIBRARIES}
	${SDL2_LIBRARIES}
	freetype
	pthread
)

list(APPEND GAME_DEFINES
	__DESKTOP__
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${EXT_LIBRARIES})

# Copy built binaries and resources for them into the `bin` directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND mkdir -p ${PROJECT_DIR}/bin)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND mv ${CMAKE_BINARY_DIR}/${PROJECT_NAME} ${PROJECT_DIR}/bin/${PROJECT_NAME})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND cp -r ${PROJECT_DIR}/resources ${PROJECT_DIR}/bin/)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND cp -r ${PROJECT_DIR}/assets ${PROJECT_DIR}/bin/)

# Prepare make clean
list(APPEND CLEAN_FILES
	${PROJECT_DIR}/bin
)
set_directory_properties(PROPERTIES
	ADDITIONAL_MAKE_CLEAN_FILES "${CLEAN_FILES}"
)
# Resources and assets paths
list(APPEND GAME_DEFINES
	RESOURCES_PATH="resources/"
	ASSETS_PATH="assets/"
)
# Set c++ definitions
set_target_properties(${PROJECT_NAME}
	PROPERTIES
	COMPILE_DEFINITIONS "${GAME_DEFINES}"
)
# Use gcc 4.8 or higher
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
