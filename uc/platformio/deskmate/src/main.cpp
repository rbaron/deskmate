#include <Arduino.h>
#include <string>
#include <vector>

#include "deskmate/arduino/gfx/sharp_mem_display.h"
#include "deskmate/arduino/input/buttons.h"
#include "deskmate/input/buttons.h"
#include "deskmate/gfx/screens/list.h"

using deskmate::arduino::gfx::SharpMemDisplay;
using deskmate::arduino::input::InputUpdater;
using deskmate::input::InputState;
using deskmate::gfx::Display;
using deskmate::gfx::screens::ListScreen;
using deskmate::gfx::screens::ListItem;

constexpr uint8_t kSCKPin = 13;
constexpr uint8_t kMOSIPin = 12;
constexpr uint8_t kCSPin = 14;
constexpr unsigned int kDisplayHeight = 240;
constexpr unsigned int kDisplayWidth = 400;

// Buttons
constexpr unsigned int kCrankPushPin = 32;

// SharpMemDisplay display(kDisplayHeight, kDisplayWidth, kSCKPin, kMOSIPin, kCSPin);
Display *display = new SharpMemDisplay(kDisplayHeight, kDisplayWidth, kSCKPin, kMOSIPin, kCSPin);
// SharpMemDisplay display(kDisplayHeight, kDisplayWidth, kSCKPin, kMOSIPin, kCSPin);

InputState input_state;
InputUpdater input_updater(
  /*crank_a_pin=*/33,
  /*crank_b_pin=*/34,
  /*crank_push_pin=*/32,
  /*a_pin=*/27,
  /*b_pin=*/26,
  /*c_pin=*/25);

// int line = 0;

std::vector<ListItem> list_items{
  ListItem{"hello1"},
  ListItem{"hello2"}
};

ListScreen list_screen(list_items);

void setup() {
  Serial.begin(9600);
  input_updater.Init();
  // display.PutText(0, 0, "Hello, world", 2, false);
  // display.PutText(0, 0, "OK!!", 2, false);
  display->Clear();
  display->Refresh();
}

void loop() {
  input_updater.Update(&input_state);
  if (input_state.GetCrankDelta()) {
    Serial.println(input_state.GetCrankDelta());
  }
  Serial.printf(
    "%d - %d - %d - %d\n",
    input_state.Crank().IsPressed(),
    input_state.A().IsPressed(),
    input_state.B().IsPressed(),
    input_state.C().IsPressed()
  );

  // if (input_state.A().WasPressed()) {
  //   // String out = String(input_state.Crank().IsPressed());
  //   std::string out;
  //   out += input_state.Crank().IsPressed() ? "1" : "0";
  //   out += input_state.A().IsPressed() ? " 1" : " 0";
  //   out += input_state.B().IsPressed() ? " 1" : " 0";
  //   out += input_state.C().IsPressed() ? " 1" : " 0";

  //   const unsigned int font_size = 2;
  //   const unsigned int line_height = 8 * font_size;
  //   if (line >= display.Height() / line_height) {
  //     display.Clear();
  //     line = 0;
  //   }
  //   display.PutText(line * line_height, 0, out, 2, false);
  //   line++;
  //   display.Refresh();
  //   // display.PutText(fff)
  //   // input_state.Crank().IsPressed();
  //       // << " - "
  //       // input_state.A().IsPressed(), input_state.B().IsPressed(), input_state.C().IsPressed()
  // }
  list_screen.Render(display);

  delay(50);
}