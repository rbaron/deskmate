#ifndef DESKMATE_ARDUINO_INPUT_BUTTONS_H
#define DESKMATE_ARDUINO_INPUT_BUTTONS_H

#include "deskmate/input/buttons.h"

namespace deskmate {
namespace arduino {
namespace input {

class InputUpdater {
 public:
  InputUpdater(
    uint8_t crank_a_pin,
    uint8_t crank_b_pin,
    uint8_t crank_push_pin,
    uint8_t a_pin,
    uint8_t b_pin,
    uint8_t c_pin):
      crank_a_pin_(crank_a_pin),
      crank_b_pin_(crank_b_pin),
      crank_push_pin_(crank_push_pin),
      a_pin_(a_pin),
      b_pin_(b_pin),
      c_pin_(c_pin) {}

  void Init();
  void Update(deskmate::input::InputState *state);
 private:
  const uint8_t crank_a_pin_, crank_b_pin_, crank_push_pin_;
  const uint8_t a_pin_, b_pin_, c_pin_;

  // Store the digital read of the crank's a input from last iteration.
  bool crank_a_last_state_;
};

}  // namespace input
}  // namespace arduino
}  // namespace deskmate

#endif  // DESKMATE_ARDUINO_INPUT_BUTTONS_H