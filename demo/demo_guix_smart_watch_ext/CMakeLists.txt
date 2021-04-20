cmake_add_library(demo)

target_sources(demo
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/weather.c
  ${CMAKE_CURRENT_SOURCE_DIR}/stopwatch.c
  ${CMAKE_CURRENT_SOURCE_DIR}/settings.c
  ${CMAKE_CURRENT_SOURCE_DIR}/message.c
  ${CMAKE_CURRENT_SOURCE_DIR}/keyboard.c
  ${CMAKE_CURRENT_SOURCE_DIR}/healthy.c
  ${CMAKE_CURRENT_SOURCE_DIR}/guix_smart_watch_specifications.c
  ${CMAKE_CURRENT_SOURCE_DIR}/demo_guix_smart_watch.c
  ${CMAKE_CURRENT_SOURCE_DIR}/custom_pixelmap_button_draw.c
  ${CMAKE_CURRENT_SOURCE_DIR}/custom_checkbox.c
  ${CMAKE_CURRENT_SOURCE_DIR}/contact_info.c
  ${CMAKE_CURRENT_SOURCE_DIR}/contact.c
  ${CMAKE_CURRENT_SOURCE_DIR}/clock_add.c
  ${CMAKE_CURRENT_SOURCE_DIR}/clock.c
  ${CMAKE_CURRENT_SOURCE_DIR}/alarm.c
  ${CMAKE_CURRENT_SOURCE_DIR}/scroll_wheel.c
)

# Resource source files
if (NOT CONFIG_GUIX_BINRES)
  target_sources(demo
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/guix_smart_watch_resources.c
  )
else()
  target_compile_definitions(demo
    PRIVATE
    -DCONFIG_GUI_SPLIT_BINRES
  )
endif()

