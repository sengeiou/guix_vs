cmake_add_library(adaptor)

target_include_directories(common_interface
  INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}
)
target_sources(adaptor
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/guix_adaptor.c
  ${CMAKE_CURRENT_SOURCE_DIR}/guix_resmgr.c
)

if (CONFIG_GUIX_BINRES)
  target_sources(adaptor
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/guix_loadres.c
  )
endif()
