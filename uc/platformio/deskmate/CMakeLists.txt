cmake_minimum_required(VERSION 3.18.4)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_FIND_DEBUG_MODE 1)

project (deskmate)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${deskmate_SOURCE_DIR}/cmake")

include_directories(lib)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(
  ${SDL2_INCLUDE_DIRS}
  ${SDL2_TTF_INCLUDE_DIRS}
)

message(STATUS "${SDL2_INCLUDE_DIRS}")
message(STATUS "${SDL2_TTF_INCLUDE_DIRS}")

set(SRC
  lib/deskmate/gfx/display.cc
  lib/deskmate/gfx/screens/list.cc
  lib/deskmate/gfx/screens/window.cc
  lib/deskmate/sdl2/gfx/display.cc
  lib/deskmate/sdl2/input/input_transcriber.cc
)

add_executable(deskmate src/main_sdl2.cc ${SRC})
target_link_libraries(deskmate ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})