cmake_minimum_required(VERSION 3.18.4)
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_FIND_DEBUG_MODE 1)

#set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")

add_compile_options(-Wall -Wextra -pedantic -Werror)

project(deskmate)

include_directories(src)

set(SRC
  src/deskmate/app/app.cc
  src/deskmate/gfx/display.cc
  src/deskmate/gfx/components/circle_horizontal_list_item.cc
  src/deskmate/gfx/components/mqtt_circle_horizontal_list_item.cc
  src/deskmate/gfx/components/mqtt_list_item.cc
  src/deskmate/gfx/components/mqtt_vertical_bar_horizontal_list_item.cc
  src/deskmate/gfx/components/vertical_bar_horizontal_list_item.cc
  src/deskmate/gfx/screens/horizontal_list.cc
  src/deskmate/gfx/screens/list.cc
  src/deskmate/gfx/screens/window.cc
)

add_library(deskmate ${SRC})
