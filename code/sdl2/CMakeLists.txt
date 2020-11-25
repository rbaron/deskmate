cmake_minimum_required(VERSION 3.18.4)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_FIND_DEBUG_MODE 1)

project (deskmate)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${deskmate_SOURCE_DIR}/cmake")

add_subdirectory(../lib/deskmate deskmate-lib-build)

include_directories(
  lib
  ../lib/deskmate/src
  third-party/paho.mqtt.c/src)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_gfx REQUIRED)

include_directories(
  src
  ${SDL2_INCLUDE_DIRS}
  ${SDL2_TTF_INCLUDE_DIRS}
  ${SDL2_GFX_INCLUDE_DIRS}
)

#message(STATUS "${SDL2_INCLUDE_DIRS}")
#message(STATUS "${SDL2_TTF_INCLUDE_DIRS}")
message(STATUS "${SDL2_GFX_INCLUDE_DIRS}")

set(SRC
  lib/deskmate/sdl2/gfx/display.cc
  lib/deskmate/sdl2/input/input_transcriber.cc
  lib/deskmate/sdl2/net/mqtt.cc
)

link_directories(${CMAKE_SOURCE_DIR}/third-party/paho.mqtt.c/build/src)

set(LIBS
  deskmate
  ${SDL2_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
  ${SDL2_GFX_LIBRARIES}
  libpaho-mqtt3cs.dylib
)

# Do we need to specify the extension .dylib here? Is there a better, cross-platform way
# of doing this?
add_executable(deskmate-sdl2 src/main.cc ${SRC})
target_link_libraries(deskmate-sdl2 ${LIBS})

# Samples
add_executable(vertical-bars-list src/samples/vertical_bars_list.cc ${SRC})
target_link_libraries(vertical-bars-list ${LIBS})

add_executable(horizontal-list src/samples/horizontal_list.cc ${SRC})
target_link_libraries(horizontal-list ${LIBS})

add_executable(simple-list src/samples/simple_list.cc ${SRC})
target_link_libraries(simple-list ${LIBS})