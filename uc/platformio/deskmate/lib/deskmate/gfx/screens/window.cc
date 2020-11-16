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
using deskmate::input::InputEvent;

// Padding inside WindowedScreens.
constexpr unsigned int kPadding = 2;

// TODO: use a circular linked list and get rid of this questionable
// implementation.
int FindNextFocusableIndex(const std::vector<WindowedScreen> wss,
                           int currently_focused) {
  // After.
  for (int i = currently_focused + 1; i < wss.size(); i++) {
    if (wss[i].focusable) {
      return i;
    }
  }
  // Before.
  for (int i = 0; i < currently_focused; i++) {
    if (wss[i].focusable) {
      return i;
    }
  }
  // No other focusable WindowedScreen was found.
  return currently_focused;
}

}  // namespace

Window::Window(std::vector<WindowedScreen>& windowed_screens)
    : windowed_screens_(windowed_screens),
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
      if (focused_index_ >= 0) {
        windowed_screens_[focused_index_].screen->HandleInputEvent(event);
      }
      break;
  }
}

void Window::Render(Display* display) const {
  for (const WindowedScreen& ws : windowed_screens_) {
    const Rect& w = ws.window;
    Rect padded_window = {
        Point{w.point.y + kPadding, w.point.x + kPadding},
        Size{w.size.height - kPadding, w.size.width - kPadding},
    };
    display->PushWindow(padded_window);
    ws.screen->Render(display);
    display->PopWindow();
  }
}

}  // namespace screens
}  // namespace gfx
}  // namespace deskmate