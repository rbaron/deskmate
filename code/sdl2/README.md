# Building

Dependencies:
* [SDL2](https://www.libsdl.org/)[SDL2] libraries. On macos, `brew install sdl2 sdl2_ttf`
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
