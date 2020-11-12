#include <Arduino.h>
#include <string>
#include <vector>

#include "deskmate/arduino/gfx/sharp_mem_display.h"
#include "deskmate/arduino/input/buttons.h"
#include "deskmate/arduino/input/crank.h"
#include "deskmate/input/input.h"
#include "deskmate/gfx/screens/list.h"

using deskmate::arduino::gfx::SharpMemDisplay;
using deskmate::arduino::input::SetupCrankInterruptHandler;
using deskmate::arduino::input::SetupButtonsInterruptHandler;
using deskmate::gfx::Display;
using deskmate::gfx::screens::ListScreen;
using deskmate::gfx::screens::ListItem;

// Input pins.
constexpr uint8_t kButtonAPin = 27;
constexpr uint8_t kButtonBPin = 26;
constexpr uint8_t kButtonCPin = 25;
constexpr uint8_t kCrankAPin = 33;
constexpr uint8_t kCrankBPin = 34;
constexpr uint8_t kCrankPushPin = 32;

// Display pins.
constexpr uint8_t kSCKPin = 13;
constexpr uint8_t kMOSIPin = 12;
constexpr uint8_t kCSPin = 14;

// Display dimensions.
constexpr unsigned int kDisplayHeight = 240;
constexpr unsigned int kDisplayWidth = 400;

Display *display = new SharpMemDisplay(kDisplayHeight, kDisplayWidth, kSCKPin, kMOSIPin, kCSPin);

std::vector<ListItem> list_items{
  ListItem{"hello1"},
  ListItem{"hello2"},
  ListItem{"hello3"},
  ListItem{"hello4"},
  ListItem{"hello5"},
  ListItem{"hello6"},
  ListItem{"hello7"},
};

ListScreen list_screen(list_items);

void setup() {
  SetupButtonsInterruptHandler(kCrankPushPin, kButtonAPin, kButtonBPin, kButtonCPin, &list_screen);
  SetupCrankInterruptHandler(kCrankAPin, kCrankBPin, &list_screen);
  Serial.begin(9600);
  display->Clear();
  display->Refresh();
}

void loop() {
  list_screen.Render(display);
  delay(50);
  Serial.println();
}