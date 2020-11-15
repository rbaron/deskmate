cmake_minimum_required(VERSION 3.18.4)
set(CMAKE_CXX_STANDARD 17)

project (deskmate)

include_directories(lib)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

message(STATUS "${SDL2_INCLUDE_DIRS}")
message(STATUS "${SDL2_LIBRARIES}")

set(SRC
  lib/deskmate/gfx/display.cc
  lib/deskmate/sdl2/gfx/display.cc
)

add_executable(deskmate src/main_sdl2.cc ${SRC})
target_link_libraries(deskmate ${SDL2_LIBRARIES})