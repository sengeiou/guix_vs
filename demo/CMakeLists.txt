
if (GUI_DEMO STREQUAL "watch")
  add_subdirectory(demo_guix_smart_watch)
  
elseif (GUI_DEMO STREQUAL "watch_ext")
  add_subdirectory(demo_guix_smart_watch_ext)

elseif (GUI_DEMO STREQUAL "keyboard")
  add_subdirectory(demo_guix_keyboard)

elseif (GUI_DEMO STREQUAL "calculator")
  add_subdirectory(demo_guix_calculator)

elseif (GUI_DEMO STREQUAL "simple")
  add_subdirectory(demo_guix_simple)
endif()