#ifndef DESKMATE_ARDUINO_INPUT_BUTTONS_H
#define DESKMATE_ARDUINO_INPUT_BUTTONS_H

#include "deskmate/input/input.h"

namespace deskmate {
namespace arduino {
namespace input {

constexpr int kDebounceMillis = 5;

void SetupButtonsInterruptHandler(uint8_t crank_pin, uint8_t a_pin,
                                  uint8_t b_pin, uint8_t c_pin,
                                  deskmate::input::InputEventHandler *handler);

}  // namespace input
}  // namespace arduino
}  // namespace deskmate

#endif  // DESKMATE_ARDUINO_INPUT_BUTTONS_H