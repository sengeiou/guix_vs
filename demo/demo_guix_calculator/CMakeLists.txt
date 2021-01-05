cmake_add_library(demo)

target_sources(demo
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/demo_guix_calculator.c
	${CMAKE_CURRENT_SOURCE_DIR}/evaluate.c
	${CMAKE_CURRENT_SOURCE_DIR}/guix_calculator_specifications.c
)

# Resource source files
if (NOT CONFIG_GUIX_BINRES)
  target_sources(demo
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/guix_calculator_resources.c
  )
endif()