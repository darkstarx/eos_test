cmake_minimum_required(VERSION 2.8)
project(eos_client)

# Project dir path
set(PROJECT_DIR ${CMAKE_SOURCE_DIR}/../..)
# Android project path
set(ANDROID_DIR ${PROJECT_DIR}/platform/android)
# JNI path
set(JNI_DIR ${ANDROID_DIR}/jni)

message(STATUS "ABI: ${ANDROID_ABI}")

# Point to our own cmake modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_DIR}/cmake_modules)

file(GLOB SRC_JNI ${JNI_DIR}/*.hpp ${JNI_DIR}/*.cpp ${ANDROID_DIR}/cpp/*.hpp ${ANDROID_DIR}/cpp/*.cpp)
file(GLOB SRC_GAME ${PROJECT_DIR}/game/*.hpp ${PROJECT_DIR}/game/*.cpp)
file(GLOB SRC_GAME_GRAPHICS ${PROJECT_DIR}/game/graphics/*.hpp ${PROJECT_DIR}/game/graphics/*.cpp)
file(GLOB SRC_GAME_UTILS ${PROJECT_DIR}/game/utils/*.hpp ${PROJECT_DIR}/game/utils/*.cpp)

set(SOURCE_FILES
	${SRC_JNI}
	${SRC_GAME}
	${SRC_GAME_GRAPHICS}
	${SRC_GAME_UTILS}
)


include_directories(
	${PROJECT_DIR}/game
	${ANDROID_DIR}/cpp
)

set(EXT_LIBRARIES
	GLESv2
	log
)

include(${PROJECT_DIR}/cmake_build/common/game.cmake)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${EXT_LIBRARIES})

# Copy built library to android/libs/armeabi and clean out debugging info
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND mkdir -p ${ANDROID_DIR}/libs/${ANDROID_ABI}/)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND cp ${LIBRARY_OUTPUT_PATH}/lib${PROJECT_NAME}.so ${ANDROID_DIR}/libs/${ANDROID_ABI}/)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_STRIP} ${ANDROID_DIR}/libs/${ANDROID_ABI}/lib${PROJECT_NAME}.so)

# Prepare make clean
list(APPEND CLEAN_FILES
	${ANDROID_DIR}/bin
	${ANDROID_DIR}/gen
)
list(APPEND ANDROID_ABIS
	armeabi
	armeabi-v7a
	x86
	mips
)
foreach(ABI IN LISTS ANDROID_ABIS)
	list(APPEND CLEAN_FILES
		${ANDROID_DIR}/obj/local/${ABI}/lib${PROJECT_NAME}.so
	)
	list(APPEND CLEAN_FILES
		${ANDROID_DIR}/libs/${ABI}
	)
endforeach()
set_directory_properties(PROPERTIES
	ADDITIONAL_MAKE_CLEAN_FILES "${CLEAN_FILES}"
)


set_target_properties(${PROJECT_NAME}
	PROPERTIES
	COMPILE_DEFINITIONS "${GAME_DEFINES}"
)

add_definitions(
    -std=c++11
)

string(TOLOWER ${CMAKE_BUILD_TYPE} JAVA_BUILD_TYPE)

set(ADB ${ANDROID_SDK}/platform-tools/adb)
set(APK ${ANDROID_DIR}/bin/EoS-${JAVA_BUILD_TYPE}.apk)

add_custom_target(build
    COMMAND ant -q ${JAVA_BUILD_TYPE}
    WORKING_DIRECTORY ${ANDROID_DIR}
)
add_custom_target(build-auto ALL
    COMMAND ant -q ${JAVA_BUILD_TYPE}
    WORKING_DIRECTORY ${ANDROID_DIR}
)
add_dependencies(build-auto ${PROJECT_NAME})

add_custom_target(install
    COMMAND ${INSTALL_WRAPPER} ${ADB} install -r ${APK}
    WORKING_DIRECTORY ${ANDROID_DIR}
)
add_custom_target(install-auto
    COMMAND ${INSTALL_WRAPPER} ${ADB} install -r ${APK}
    WORKING_DIRECTORY ${ANDROID_DIR}
)
add_dependencies(install-auto build-auto)


add_custom_target(clearlog
    COMMAND ${ADB} logcat -c
    WORKING_DIRECTORY ${ANDROID_DIR}
)
add_custom_target(clearlog-auto
    COMMAND ${ADB} logcat -c
    WORKING_DIRECTORY ${ANDROID_DIR}
)
add_dependencies(clearlog-auto install-auto)

add_custom_target(start
    COMMAND ${ADB} shell am start -a android.intent.action.MAIN -c [android.intent.category.LAUNCHER] -n ${PACKAGE}/.${ACTIVITY}
    WORKING_DIRECTORY ${ANDROID_DIR}
)
add_custom_target(start-auto
    COMMAND ${ADB} shell am start -a android.intent.action.MAIN -c [android.intent.category.LAUNCHER] -n ${PACKAGE}/.${ACTIVITY}
    WORKING_DIRECTORY ${ANDROID_DIR}
)
add_dependencies(start-auto clearlog-auto)

add_custom_target(logcat
    COMMAND ${ADB} logcat -s -v threadtime ${FILTER}
    WORKING_DIRECTORY ${ANDROID_DIR}
)
add_custom_target(logcat-auto
    COMMAND ${ADB} logcat -s -v threadtime ${FILTER}
    WORKING_DIRECTORY ${ANDROID_DIR}
)
add_dependencies(logcat-auto start-auto)

add_custom_target(full)
add_dependencies(full logcat-auto)

add_custom_target(uninstall
    COMMAND ${ADB} uninstall ${PACKAGE}
    WORKING_DIRECTORY ${ANDROID_DIR}
)