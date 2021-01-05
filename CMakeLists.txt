cmake_minimum_required(VERSION 3.8)
project(guix)
enable_language(C CXX)


# Configure options
option(CONFIG_GUIX_BINRES "Split binary resource from executable file" OFF)


# Generate configure file
configure_file(${PROJECT_SOURCE_DIR}/config.h.in 
               ${PROJECT_SOURCE_DIR}/config.h 
               @ONLY)

# Select operation system
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  set(__win32 1)
  message(STATUS "OS Platform: ${CMAKE_SYSTEM_NAME}")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(__linux 1)
  message(STATUS "OS Platform: ${CMAKE_SYSTEM_NAME}")
else ()
  message(FATAL_ERROR "Unknown operation system")
endif ()

# Find package
if (__linux)
  include(FindPkgConfig)
  pkg_search_module(SDL2 REQUIRED sdl2)

  include_directories(
    ${SDL2_INCLUDE_DIR}
  )
elseif (__win32)
  set(SDL_PATH sdl/SDL2/lib/x86)
  link_directories(${SDL_PATH})
  list(APPEND SDL2_LIBRARIES 
       SDL2
       SDL2main
  )
  if (NOT EXISTS ${PROJECT_BINARY_DIR}/SDL2.dll)
    file(COPY 
      ${SDL_PATH}/SDL2.dll 
      DESTINATION 
      ${PROJECT_BINARY_DIR})
  endif()
endif()

# Set target name
if (__linux)
  set(EXE_FILE ${PROJECT_NAME}.elf)
  add_definitions(-include config.h)
elseif (__win32)
  set(EXE_FILE ${PROJECT_NAME}.exe)
  add_definitions(/FI"config.h")
endif()

macro(cmake_add_library name)
  add_library(${name} STATIC "")
  set_property(GLOBAL APPEND PROPERTY common_libs ${name})
  target_link_libraries(${name} PUBLIC common_interface)
endmacro()

# Global compile options
include_directories(
  ${PROJECT_SOURCE_DIR}
)

# Common interface library
add_library(common_interface INTERFACE )

# Generate executable file
add_executable(${EXE_FILE} empty.c)

# Add subdirectories
add_subdirectory(demo)
add_subdirectory(adaptor)
add_subdirectory(lib)
if (__win32)
  add_subdirectory(sdl)
endif(__win32)

# Link target
get_property(libs GLOBAL PROPERTY common_libs)
foreach(current_lib ${libs})
  add_dependencies(${EXE_FILE} ${current_lib})
  message(STATUS "-> ${current_lib}")
endforeach()

target_link_libraries(${EXE_FILE} 
  ${libs}
  ${SDL2_LIBRARIES}
)

# Debug command
if (__linux)
  set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
  add_custom_target(run 
    COMMAND 
    ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}
  )
  add_custom_target(ddd 
    COMMAND 
    ddd --debugger ${CMAKE_DEBUGER} ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}
  )
  add_custom_target(insight 
    COMMAND 
    insight ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}
  )
  add_custom_target(gdb 
    COMMAND 
    {CMAKE_DEBUGER} ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}
  )
endif(__linux)