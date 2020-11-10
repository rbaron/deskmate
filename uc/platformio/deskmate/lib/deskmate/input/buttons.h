#ifndef DESKMATE_INPUT_BUTTONS_H
#define DESKMATE_INPUT_BUTTONS_H

#include <string>

namespace deskmate {
namespace input {

enum class Buttons {
  kCrankDelta,
  kCrankPush,
  kA,
  kB,
  kC,
};

class ButtonState {
 public:
  void Set(bool pushed) {
    toggled_ = pushed != pushed_;
    pushed_ = pushed;
  }
  bool IsPressed() const {
    return pushed_;
  }
  bool WasPressed() const {
    return pushed_ && toggled_;
  }
  bool WasReleased() const {
    return !pushed_ && toggled_;
  }
 private:
  bool pushed_;
  bool toggled_;
};

class InputState {
 public:
  int GetCrankDelta() const {
    return crank_delta_;
  }
  const ButtonState& Crank() const {
    return crank_;
  }
  const ButtonState& A() const {
    return a_;
  }
  const ButtonState& B() const {
    return b_;
  }
  const ButtonState& C() const {
    return c_;
  }
  void SetCrankDelta(int delta) {
    crank_delta_ = delta;
  }
  ButtonState& Crank() {
    return crank_;
  }
  ButtonState& A() {
    return a_;
  }
  ButtonState& B() {
    return b_;
  }
  ButtonState& C() {
    return c_;
  }
 private:
  int crank_delta_;
  ButtonState crank_, a_, b_, c_;
};

}  // namespace input
}  // namespace deskmate

#endif  // DESKMATE_INPUT_BUTTONS_H