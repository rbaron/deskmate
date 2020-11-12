#include <Arduino.h>

#include <string>
#include <vector>

#include "deskmate/arduino/gfx/sharp_mem_display.h"
#include "deskmate/arduino/input/buttons.h"
#include "deskmate/arduino/input/crank.h"
#include "deskmate/gfx/screens/list.h"
#include "deskmate/input/input.h"

using deskmate::arduino::gfx::SharpMemDisplay;
using deskmate::arduino::input::SetupButtonsInterruptHandler;
using deskmate::arduino::input::SetupCrankInterruptHandler;
using deskmate::gfx::Display;
using deskmate::gfx::screens::ListItem;
using deskmate::gfx::screens::ListScreen;

// Input pins.
constexpr uint8_t kButtonAPin = 27;
constexpr uint8_t kButtonBPin = 26;
constexpr uint8_t kButtonCPin = 25;
constexpr uint8_t kCrankAPin = 33;
constexpr uint8_t kCrankBPin = 34;
constexpr uint8_t kCrankPushPin = 32;

// Display pins.
constexpr uint8_t kSCKPin = 14;
constexpr uint8_t kMOSIPin = 13;
constexpr uint8_t kCSPin = 15;

// Display dimensions.
constexpr unsigned int kDisplayHeight = 240;
constexpr unsigned int kDisplayWidth = 400;

Display* display = new SharpMemDisplay(kDisplayHeight, kDisplayWidth, kSCKPin,
                                       kMOSIPin, kCSPin);

class TalkyListItem : public ListItem {
 public:
  explicit TalkyListItem(const std::string& name) : name_(name) {}
  std::string Render() const override { return name_; }
  void OnSelect() override { Serial.printf("%s Pressed!\n", name_.c_str()); }

 private:
  std::string name_;
};

std::unique_ptr<ListScreen> list_screen;

void setup() {
  std::vector<std::unique_ptr<ListItem>> list_items;
  for (int i = 0; i < 15; i++) {
    String a("item #");
    a += String(i);
    list_items.push_back(std::make_unique<TalkyListItem>(a.c_str()));
  }
  list_screen = std::make_unique<ListScreen>(list_items);
  SetupButtonsInterruptHandler(kCrankPushPin, kButtonAPin, kButtonBPin,
                               kButtonCPin, list_screen.get());
  SetupCrankInterruptHandler(kCrankAPin, kCrankBPin, list_screen.get());
  Serial.begin(9600);
}

void loop() {
  auto before = millis();
  list_screen->Render(display);
  delay(10);
  // Serial.println();
}