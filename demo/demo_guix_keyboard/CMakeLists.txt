cmake_add_library(demo)

target_sources(demo
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/demo_guix_keyboard.c
	${CMAKE_CURRENT_SOURCE_DIR}/guix_keyboard_resources.c
	${CMAKE_CURRENT_SOURCE_DIR}/guix_keyboard_specifications.c
)
