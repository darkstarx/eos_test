cmake_minimum_required(VERSION 2.8)
project(eos_client)

# Project dir path
set(PROJECT_DIR ${CMAKE_SOURCE_DIR}/../..)

# Point to our own cmake modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_DIR}/cmake_modules)

file(GLOB SRC_ROOT ${PROJECT_DIR}/platform/linux/*.h ${PROJECT_DIR}/platform/linux/*.cpp)
file(GLOB SRC_GAME ${PROJECT_DIR}/game/*.h ${PROJECT_DIR}/game/*.cpp)
file(GLOB SRC_GAME_GRAPHICS ${PROJECT_DIR}/game/graphics/*.h ${PROJECT_DIR}/game/graphics/*.cpp)
file(GLOB SRC_GAME_UTILS ${PROJECT_DIR}/game/utils/*.h ${PROJECT_DIR}/game/utils/*.cpp)

set(SOURCE_FILES
	${SRC_ROOT}
	${SRC_GAME}
	${SRC_GAME_GRAPHICS}
	${SRC_GAME_UTILS}
)

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

# google-glog
find_package(GLog)
if (NOT GLOG_FOUND)
	set(GLOG_LIB_DIR ${CMAKE_BINARY_DIR}/libs/glog/lib)
	message(STATUS "Searching for GLog library: ${GLOG_LIB_DIR}")
	find_library(GLOG_LIBRARIES
		NAMES glog
		PATHS ${GLOG_LIB_DIR}
	)
	if (NOT GLOG_LIBRARIES)
		message(STATUS "Searching for GLog library: ${GLOG_LIB_DIR} -- not found")
		# google-glog compilation and linking libglog.so
		message(STATUS "Building GLog in ${CMAKE_BINARY_DIR}/libs/glog-compile...")
		execute_process(COMMAND mkdir -p ${CMAKE_BINARY_DIR}/libs/glog-compile)
		execute_process(
			COMMAND ${PROJECT_DIR}/lib/glog/configure --prefix=${CMAKE_BINARY_DIR}/libs/glog
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/libs/glog-compile
			OUTPUT_QUIET
			OUTPUT_FILE "configure.log"
		)
		message(STATUS "Installing GLog in ${CMAKE_BINARY_DIR}/libs/glog...")
		execute_process(
			COMMAND make install -j4
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/libs/glog-compile
			OUTPUT_QUIET
		)
		if(CMAKE_SIZEOF_VOID_P EQUAL 8)
			set(GLOG_LIBRARIES ${CMAKE_BINARY_DIR}/libs/glog/lib64/libglog.so)
		else(CMAKE_SIZEOF_VOID_P EQUAL 8)
			set(GLOG_LIBRARIES ${CMAKE_BINARY_DIR}/libs/glog/lib/libglog.so)
		endif(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(GLOG_INCLUDE_DIR ${CMAKE_BINARY_DIR}/libs/glog/include)
	endif (NOT GLOG_LIBRARIES)
endif (NOT GLOG_FOUND)
message(STATUS "GLog includes: ${GLOG_INCLUDE_DIR} Libraries: ${GLOG_LIBRARIES}" )


include_directories(
	${PROJECT_DIR}/game
	${GLOG_INCLUDE_DIR}
)

set(EXT_LIBRARIES
	${OPENGL_LIBRARIES}
	${SDL2_LIBRARIES}
	${GLOG_LIBRARIES}
)

include(${PROJECT_DIR}/cmake_build/common/game.cmake)


list(APPEND GAME_DEFINES
	__DESKTOP__
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${EXT_LIBRARIES})

set_target_properties(${PROJECT_NAME}
	PROPERTIES
	COMPILE_DEFINITIONS "${GAME_DEFINES}"
)
