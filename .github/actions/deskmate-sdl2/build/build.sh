#!/bin/ash
set -eux -o pipefail

# Builds paho.mqtt.c. This assumes git submodules were already fetched.
cd "$GITHUB_WORKSPACE"/code/sdl2/third-party/paho.mqtt.c
mkdir build
cd build
cmake -GNinja -DPAHO_WITH_SSL=TRUE -DPAHO_BUILD_SAMPLES=TRUE ..
ninja

# Builds deskmate-sdl2
cd "$GITHUB_WORKSPACE"/code/sdl2
cp src/credentials_example.h src/credentials.h
mkdir build
cd build
cmake ..
make