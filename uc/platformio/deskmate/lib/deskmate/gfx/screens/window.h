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
using deskmate::gfx::Size;
using deskmate::input::InputEvent;
}  // namespace

struct WindowedScreen {
  Screen* screen;
  Rect window;
};

// Capable of rendering multiple Screens. But it is also itself a Screen.
class Window : public Screen {
 public:
  // Window is constructed with a vector of non-owned pointers to Screens.
  explicit Window(std::vector<WindowedScreen>& windowed_screens)
      : windowed_screens_(windowed_screens) {}
  ~Window() override;
  void HandleInputEvent(InputEvent event) override;
  void Render(Display* display) const override;

 private:
  std::vector<WindowedScreen> windowed_screens_;
};

}  // namespace screens
}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_SCREENS_WINDOW_H