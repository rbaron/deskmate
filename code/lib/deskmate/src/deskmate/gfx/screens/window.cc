#include "deskmate/gfx/screens/window.h"

#include <algorithm>

#include "deskmate/gfx/display.h"
#include "deskmate/input/input.h"

namespace deskmate {
namespace gfx {
namespace screens {

namespace {
using deskmate::gfx::Color;
using deskmate::gfx::Rect;
using deskmate::gfx::Size;
using deskmate::input::InputEvent;

// Padding inside WindowedScreens.
constexpr int kPadding = 1;
constexpr int kFocusMarkWidth = 12;

int FindNextFocusableIndex(const std::vector<WindowedScreen>& wss,
                           int focused_index) {
  // Cycle through focusable windows. Increment focused_index (mod N) until it
  // points to a focusable window.
  while (!wss[(++focused_index) % wss.size()].focusable)
    ;
  return focused_index % wss.size();
}

}  // namespace

Window::Window(std::vector<WindowedScreen>& windowed_screens)
    : windowed_screens_(std::move(windowed_screens)),
      focused_index_(FindNextFocusableIndex(windowed_screens_, -1)) {}

Window::~Window() {}

void Window::HandleInputEvent(InputEvent event) {
  switch (event) {
    // Pressing B cycles through focusable windows.
    case InputEvent::kBPush:
      focused_index_ =
          FindNextFocusableIndex(windowed_screens_, focused_index_);
      break;
    // Forward all other inputs to the focused WindowedScreen (if any).
    default:
      windowed_screens_[focused_index_].screen->HandleInputEvent(event);
      break;
  }
}

void Window::Render(Display* display) const {
  // for (const WindowedScreen& ws : windowed_screens_) {
  for (std::size_t i = 0; i < windowed_screens_.size(); i++) {
    const WindowedScreen& ws = windowed_screens_[i];
    const Rect& w = ws.window;
    Rect padded_window = {
        Point{w.point.y + kPadding, w.point.x + kPadding},
        Size{w.size.height - kPadding, w.size.width - kPadding},
    };
    display->PushWindow(padded_window);
    ws.screen->Render(display);
    display->PopWindow();

    // If focused, draw a little square on the top right.
    if (i == focused_index_) {
      Rect focus_mark_rect{w.point + Point{0, w.size.width - kFocusMarkWidth},
                           Size{kFocusMarkWidth, kFocusMarkWidth}};
      display->FillRect(focus_mark_rect, Color::kBlack);
    }

    // Draw the border after the content has been drawn.
    display->DrawRect(w, Color::kBlack);
  }
}

}  // namespace screens
}  // namespace gfx
}  // namespace deskmate