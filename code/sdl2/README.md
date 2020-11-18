# Notes
## Compiling paho.mqtt.c
```
$ git submodule init --update
$ cd third-party/paho.mqtt.c
$ mkdir build
$ cd build
$ cmake -GNinja -DPAHO_WITH_SSL=TRUE -DPAHO_BUILD_SAMPLES=TRUE -DOPENSSL_ROOT_DIR=$HOME/homebrew/opt/openssl@1.1/ ..
$ ninja
```
