#ifndef DESKMATE_GFX_SCREEN_H
#define DESKMATE_GFX_SCREEN_H

#include "deskmate/input/input.h"
#include "deskmate/gfx/display.h"

namespace deskmate {
namespace gfx {

class Screen : public deskmate::input::InputEventHandler {
 public:
  virtual ~Screen() = default;
  virtual void HandleInputEvent(deskmate::input::InputEvent input_event) = 0;
  virtual void Render(Display *display) const = 0;
};

}  // namespace deskmate
}  // namespace gfx

#endif  // DESKMATE_GFX_SCREEN_H