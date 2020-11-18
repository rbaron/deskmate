#include "deskmate/arduino/input/crank.h"

#include <Arduino.h>

#include "deskmate/input/input.h"

namespace deskmate {
namespace arduino {
namespace input {
namespace {

using deskmate::input::InputEvent;
using deskmate::input::InputEventHandler;

deskmate::input::InputEventHandler *input_handler = nullptr;

struct CrankPin {
  int pin_n;
  volatile int pin_value;
};

CrankPin crank_a{-1, HIGH};
CrankPin crank_b{-1, HIGH};

class State {
 public:
  enum States {
    kInitial = 0,
    k1CW,
    k2CW,
    k3CW,
    k1CCW,
    k2CCW,
    k3CCW,
  };
};
State::States state = State::kInitial;

State::States transition_table[][4] = {
    // 00              01                10               11
    {State::kInitial, State::k1CCW, State::k1CW,
     State::kInitial},  // State::kInitial

    // Clockwise transitions.
    {State::kInitial, State::kInitial, State::k1CW,
     State::k2CW},  // State::k1CW
    {State::kInitial, State::k3CW, State::kInitial,
     State::k2CW},  // State::k2CW
    {State::kInitial, State::k3CW, State::kInitial,
     State::kInitial},  // State::k3CW

    // Counter-clockwise transitions.
    {State::kInitial, State::k1CCW, State::kInitial,
     State::k2CCW},  // State::k1CCW
    {State::kInitial, State::kInitial, State::k3CCW,
     State::k2CCW},  // State::k2CCW
    {State::kInitial, State::kInitial, State::k3CCW,
     State::kInitial},  // State::k3CCW
};

void HandleStateTransition() {
  int transition =
      ((crank_a.pin_value == LOW) << 1 | crank_b.pin_value == LOW) & 0x3;
  State::States next = transition_table[state][transition];
  if (state == State::k3CW && next == State::kInitial) {
    input_handler->HandleInputEvent(InputEvent::kCrankCW);
  } else if (state == State::k3CCW && next == State::kInitial) {
    input_handler->HandleInputEvent(InputEvent::kCrankCCW);
  }
  state = next;
}

void ISRCrankA() {
  crank_a.pin_value = digitalRead(crank_a.pin_n);
  HandleStateTransition();
}

void ISRCrankB() {
  crank_b.pin_value = digitalRead(crank_b.pin_n);
  HandleStateTransition();
}

}  // namespace

void SetupCrankInterruptHandler(uint8_t crank_a_pin, uint8_t crank_b_pin,
                                InputEventHandler *handler) {
  input_handler = handler;
  crank_a.pin_n = crank_a_pin;
  crank_b.pin_n = crank_b_pin;
  pinMode(crank_a.pin_n, INPUT);
  pinMode(crank_b.pin_n, INPUT);
  attachInterrupt(crank_a.pin_n, ISRCrankA, CHANGE);
  attachInterrupt(crank_b.pin_n, ISRCrankB, CHANGE);
}

}  // namespace input
}  // namespace arduino
}  // namespace deskmate