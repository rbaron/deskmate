# deskmate-arduino
This is a [PlatformIO](https://platformio.org/) project that adds Arduino implementation to the platform agnostic `deskmate` library. The hardware I currently use is:
* SHARP Memory Display, 2.7" [Adafruit](https://www.adafruit.com/product/4694)
* [Lolin32 ESP32](https://docs.platformio.org/en/latest/boards/espressif32/lolin32.html) development board

Ease of adding support for different hardware is a major project goal. For example, [this is how](lib/deskmate/arduino/gfx/sharp_mem_display.h) the SHARP display implements the `Display` interface. Adding support to a new display amounts to implementing this same interface for it.

As a matter of fact, this same `Display` interface is implement in the [`deskmate-sdl2` project](../sdl2). The implementation is [here](../sdl2/lib/deskmate/sdl2/gfx/display.h). This implement a software display using the SDL library.

## Build & run
The easiest way to build, run and upload `deskmate-arduino` to your board is to install the PlatformIO extension to your favorite IDE and open this directory. In my case, I use [PlatformIO for VSCode](https://platformio.org/install/ide?install=vscode).
