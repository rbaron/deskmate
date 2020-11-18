#ifndef DESKMATE_INPUT_BUTTONS_H
#define DESKMATE_INPUT_BUTTONS_H

#include <string>

namespace deskmate {
namespace input {

enum class InputEvent {
  kUnknown,
  kCrankCW,
  kCrankCCW,
  kCrankPush,
  kCrankRelease,
  kAPush,
  kARelease,
  kBPush,
  kBRelease,
  kCPush,
  kCRelease,
};

class InputEventHandler {
 public:
  virtual ~InputEventHandler() = default;
  virtual void HandleInputEvent(InputEvent event) = 0;
};

}  // namespace input
}  // namespace deskmate

#endif  // DESKMATE_INPUT_BUTTONS_H