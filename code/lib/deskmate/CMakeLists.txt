cmake_minimum_required(VERSION 3.18.4)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_FIND_DEBUG_MODE 1)

project(deskmate)

include_directories(src)

set(SRC
  src/deskmate/gfx/display.cc
  src/deskmate/gfx/screens/list.cc
  src/deskmate/gfx/screens/window.cc
  src/deskmate/mqtt/mqtt.cc
  src/deskmate/app/app.cc
)

add_library(deskmate ${SRC})
