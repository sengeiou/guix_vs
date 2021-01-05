cmake_add_library(demo)

target_sources(demo
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/demo_guix_simple.c
	${CMAKE_CURRENT_SOURCE_DIR}/guix_simple_resources.c
	${CMAKE_CURRENT_SOURCE_DIR}/guix_simple_specifications.c
)
