cmake_minimum_required(VERSION 3.18.4)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_FIND_DEBUG_MODE 1)

project(deskmate)

include_directories(src)

set(SRC
  src/deskmate/app/app.cc
  src/deskmate/gfx/display.cc
  src/deskmate/gfx/components/circle_horizontal_list_item.cc
  src/deskmate/gfx/components/mqtt_circle_horizontal_list_item.cc
  src/deskmate/gfx/components/mqtt_list_item.cc
  src/deskmate/gfx/components/mqtt_vertical_bar_list_item.cc
  src/deskmate/gfx/screens/horizontal_list.cc
  src/deskmate/gfx/screens/list.cc
  src/deskmate/gfx/screens/vertical_bars_list.cc
  src/deskmate/gfx/screens/window.cc
  src/deskmate/mqtt/mqtt.cc
)

add_library(deskmate ${SRC})
