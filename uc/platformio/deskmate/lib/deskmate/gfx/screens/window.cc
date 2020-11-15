#include "deskmate/gfx/screens/window.h"

#include <Arduino.h>

#include <algorithm>

#include "deskmate/gfx/display.h"
#include "deskmate/input/input.h"

namespace deskmate {
namespace gfx {
namespace screens {

namespace {
using deskmate::gfx::Color;
using deskmate::input::InputEvent;
}  // namespace

Window::~Window() {}

void Window::HandleInputEvent(InputEvent event) {
  // TODO: forward key presses to focused screen.
  // switch (event) {
  //   case InputEvent::kCrankCW:
  //     selected_ = selected_ < items_.size() - 1 ? selected_ + 1 : selected_;
  //     dirty_ = true;
  //     break;
  //   case InputEvent::kCrankCCW:
  //     selected_ = selected_ > 0 ? selected_ - 1 : 0;
  //     dirty_ = true;
  //     break;
  //   case InputEvent::kAPush:
  //     items_[selected_]->OnSelect();
  //     break;
  //   default:
  //     break;
  // }
}

void Window::Render(Display* display) const {
  for (const WindowedScreen& ws : windowed_screens_) {
    Serial.printf("Will render -> (%ud, %ud)\n", ws.window.point.y, ws.window.point.x);
    display->SetWindow(ws.window);
    ws.screen->Render(display);
  }
}

}  // namespace screens
}  // namespace gfx
}  // namespace deskmate