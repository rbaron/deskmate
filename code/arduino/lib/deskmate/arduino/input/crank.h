#ifndef DESKMATE_ARDUINO_INPUT_CRANK_H
#define DESKMATE_ARDUINO_INPUT_CRANK_H

#include "deskmate/input/input.h"

namespace deskmate {
namespace arduino {
namespace input {

void SetupCrankInterruptHandler(uint8_t crank_a_pin, uint8_t crank_b_pin,
                                deskmate::input::InputEventHandler *handler);

}  // namespace input
}  // namespace arduino
}  // namespace deskmate

#endif  // DESKMATE_ARDUINO_INPUT_CRANK_H