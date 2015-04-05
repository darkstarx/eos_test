include_directories(
	${PROJECT_DIR}/lib/utf8-cpp
)

list(APPEND GAME_DEFINES
	LOG_TAG="${PROJECT_NAME}"
	GLM_FORCE_RADIANS
)
