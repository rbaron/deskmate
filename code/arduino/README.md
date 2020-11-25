# deskmate-arduino
This is a [PlatformIO](https://platformio.org/) project that adds Arduino implementation to the platform-agnostic `deskmate` library.

The hardware I currently use is:
* SHARP Memory Display, 2.7" [Adafruit](https://www.adafruit.com/product/4694)
* [Lolin32 ESP32](https://docs.platformio.org/en/latest/boards/espressif32/lolin32.html) development board

Ease of porting this implementation to different hardware is a major goal. For example, [this is how](lib/deskmate/arduino/gfx/sharp_mem_display.h) the SHARP display implements the abstract `Display` interface. Adding support to a new display amounts to implementing this same interface for it.

## Configuration
The entrypoint ([src/main.cc](src/main.cc)) expects a `credentials.h` file under the `src/` directory. Copy the `src/credentials_example.h` and fill it up with your configuration.

## Build & run
The easiest way to build, run and upload `deskmate-arduino` to your board is to install the PlatformIO extension to your favorite IDE and open this directory. In my case, I use [PlatformIO for VSCode](https://platformio.org/install/ide?install=vscode).
