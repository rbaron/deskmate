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

struct WindowedScreen {
  // Screen* screen;
  std::unique_ptr<deskmate::gfx::Screen> screen;
  deskmate::gfx::Rect window;
  // Whether or not this WindowedScreen can be "focused". A focused
  // WindowsScreen is one that the Window will pipe (some) InputEvents to. For
  // example, a status bar might not be focusable, since it's read only.
  bool focusable;
};

// Capable of rendering multiple Screens. But it is also itself a Screen.
// The Window owns the unique_ptrs or its children Screens.
// TODO: change name to SplitScreen.
class Window : public Screen {
 public:
  // Window is constructed with a vector of non-owned pointers to Screens.
  explicit Window(std::vector<WindowedScreen>& windowed_screens);
  ~Window() override;
  void HandleInputEvent(deskmate::input::InputEvent event) override;
  void Render(Display* display) const override;

 private:
  std::vector<WindowedScreen> windowed_screens_;

  // Points to the currently focused WindowScreen.
  std::size_t focused_index_ = -1;
};

}  // namespace screens
}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_SCREENS_WINDOW_H