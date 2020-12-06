#include "deskmate/arduino/input/buttons.h"

#include <Arduino.h>

namespace deskmate {
namespace arduino {
namespace input {

namespace {

using deskmate::input::InputEvent;
using deskmate::input::InputEventHandler;

InputEventHandler *input_handler = nullptr;

void HandleButtonPush(InputEvent event) {
  if (input_handler != nullptr) {
    input_handler->HandleInputEvent(event);
  }
}

// Approach 1: not very reliable. Using digitalRead does not return
// the value I would expect, specially on button release. I would
// expect for the first value to be HIGH, but sometimes it is low.
// How to reproduce: Push and keep the button push. Wait for a second,
// release the button.
// The problem is timing: between the CHANGE triggering the ISR and
// it actually running, the spurious bounces might have changed the
// input to the port.
// void ISRAPush() {
//   int value = digitalRead(27);
//   Serial.printf("Value: %s\n", value == LOW ? "LOW" : "HIGH");
//   static  long last_millis = 0;
//    long now = millis();
//   if (now - last_millis > 100) {
//     if (value == LOW) {
//       Serial.println("Pressed!");
//       // HandleButtonPush(deskmate::input::InputEvent::kAPush);
//     } else {
//       Serial.println("Released!");
//     }
//   }
//   last_millis = now;
// }

// Approach #2. Only HIGH. Sort of works, but only considers pushes,
// no releases, and fires a button press upon release, if you hold it
// for more than kDebounceMillis. Not good.
// void ISRAPush() {
//   static  long last_millis = 0;
//    long now = millis();
//   if (now - last_millis > 100) {
//     Serial.println("Pressed!");
//   }
//   last_millis = now;
// }

// Approach #3. Keep an internal state of the button. This seems to work
// best, with a very short ~5ms debounce.
// Some possible bugs:
// - If pressed and released inside 5ms, the state will be forever wrong
//   (or until a new sub-5ms press-release);
// - If the program starts with the button pressed, the state will be
//   wrong;
// In practice, this seems to work well enough, though.
// void ISRAPush() {
//   static bool pushed = false;
//   static  long last_millis = 0;
//    long now = millis();
//   if (now - last_millis > 5) {
//     if (!pushed) {
//       Serial.println("Pressed!");
//     } else {
//       Serial.println("Released!");
//     }
//     pushed = !pushed;
//   }
//   last_millis = now;
// }

#define DEBOUNCED_ISR(NAME, PUSH_EVENT, RELEASE_EVENT) \
  void NAME() {                                        \
    static bool pushed = false;                        \
    static long last_millis = 0;                       \
    long now = millis();                               \
    if (now - last_millis > kDebounceMillis) {         \
      if (!pushed) {                                   \
        HandleButtonPush(PUSH_EVENT);                  \
      } else {                                         \
        HandleButtonPush(RELEASE_EVENT);               \
      }                                                \
      pushed = !pushed;                                \
    }                                                  \
    last_millis = now;                                 \
  }

DEBOUNCED_ISR(ISRCrankButton, InputEvent::kCrankPush,
              InputEvent::kCrankRelease);
DEBOUNCED_ISR(ISRAButton, InputEvent::kAPush, InputEvent::kARelease);
DEBOUNCED_ISR(ISRBButton, InputEvent::kBPush, InputEvent::kBRelease);
DEBOUNCED_ISR(ISRCButton, InputEvent::kCPush, InputEvent::kCRelease);

}  // namespace

void SetupButtonsInterruptHandler(uint8_t crank_pin, uint8_t a_pin,
                                  uint8_t b_pin, uint8_t c_pin,
                                  deskmate::input::InputEventHandler *handler) {
  input_handler = handler;
  pinMode(crank_pin, INPUT_PULLUP);
  pinMode(a_pin, INPUT_PULLUP);
  pinMode(b_pin, INPUT_PULLUP);
  pinMode(c_pin, INPUT_PULLUP);

  attachInterrupt(crank_pin, ISRCrankButton, CHANGE);
  attachInterrupt(a_pin, ISRAButton, CHANGE);
  attachInterrupt(b_pin, ISRBButton, CHANGE);
  attachInterrupt(c_pin, ISRCButton, CHANGE);
}

}  // namespace input
}  // namespace arduino
}  // namespace deskmate