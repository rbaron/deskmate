#include <Arduino.h>

#include "deskmate/input/buttons.h"
#include "deskmate/arduino/input/buttons.h"

namespace deskmate {
namespace arduino {
namespace input {

void InputUpdater::Init() {
  pinMode(crank_push_pin_, INPUT);
  pinMode(crank_a_pin_, INPUT);
  pinMode(crank_b_pin_, INPUT);
  pinMode(a_pin_, INPUT_PULLUP);
  pinMode(b_pin_, INPUT_PULLUP);
  pinMode(c_pin_, INPUT_PULLUP);
  crank_a_last_state_ = digitalRead(crank_a_pin_);
}

void InputUpdater::Update(deskmate::input::InputState *state) {
  // Buttons are active low (normally pulled up).
  state->Crank().Set(digitalRead(crank_push_pin_) == LOW);
  state->A().Set(digitalRead(a_pin_) == LOW);
  state->B().Set(digitalRead(b_pin_) == LOW);
  state->C().Set(digitalRead(c_pin_) == LOW);

  int crank_a_state = digitalRead(crank_a_pin_);
  if (crank_a_state != crank_a_last_state_) {
    state->SetCrankDelta(
      digitalRead(crank_b_pin_) != crank_a_state ? 1 : -1);
  } else {
    state->SetCrankDelta(0);
  }
  crank_a_last_state_ = crank_a_state;
}

}  // namespace input
}  // namespace arduino
}  // namespace deskmate