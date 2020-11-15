#ifndef DESKMATE_GFX_SCREENS_WINDOW_H
#define DESKMATE_GFX_SCREENS_WINDOW_H

#include <memory>
#include <vector>

#include "deskmate/gfx/display.h"
#include "deskmate/gfx/screen.h"
#include "deskmate/input/input.h"

namespace deskmate {
namespace gfx {
namespace screens {
namespace {
using deskmate::gfx::Rect;
using deskmate::gfx::Screen;
using deskmate::gfx::Size;
using deskmate::input::InputEvent;
}  // namespace

struct WindowedScreen {
  Screen* screen;
  Rect window;
  // Whether or not this WindowedScreen can be "focused". A focused
  // WindowsScreen is one that the Window will pipe (some) InputEvents to. For
  // example, a status bar might not be focusable, since it's read only.
  bool focusable;
};

// Capable of rendering multiple Screens. But it is also itself a Screen.
class Window : public Screen {
 public:
  // Window is constructed with a vector of non-owned pointers to Screens.
  explicit Window(std::vector<WindowedScreen>& windowed_screens);
  ~Window() override;
  void HandleInputEvent(InputEvent event) override;
  void Render(Display* display) const override;

 private:
  std::vector<WindowedScreen> windowed_screens_;

  // Points to the currently focused WindowScreen.
  // TODO: use a circular list.
  int focused_index_ = -1;
};

}  // namespace screens
}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_SCREENS_WINDOW_H