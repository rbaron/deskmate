# Building

Dependencies:
* [SDL2](https://www.libsdl.org/)[SDL2] libraries. On macos, `brew install sdl2 sdl2_ttf sdl2_gfx`
* [Eclipse Paho C Library](https://www.eclipse.org/paho/). See below how to build it.

## Step 1. Build paho.mqtt.c
```
$ git submodule init --update
$ cd third-party/paho.mqtt.c
$ mkdir build
$ cd build
$ cmake -GNinja -DPAHO_WITH_SSL=TRUE -DPAHO_BUILD_SAMPLES=TRUE -DOPENSSL_ROOT_DIR=$HOME/homebrew/opt/openssl@1.1/ ..
$ ninja
```

## Step 2. Configure it
Copy `credentials_template.h` into `credentials.h` and edit it. It contains MQTT credentials and sensors/switches specifications.

## Step 3. Build `deskmate-sdl2`

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Step 3. Run it!
```
$ ./deskmate-sdl2
```

# License
The `deskmate-sdl2` is distributed under the MIT license (see `LICENSE` in the root directory of this repo).

Files in the `cmake/` directory are distrubited under the BSD 3-Clause License and were obtained from [github.com/aminosbh/sdl2-cmake-modules](https://github.com/aminosbh/sdl2-cmake-modules).