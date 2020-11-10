#ifndef DESKMATE_GFX_SCREEN_H
#define DESKMATE_GFX_SCREEN_H

#include "deskmate/input/buttons.h"
#include "deskmate/gfx/display.h"

namespace deskmate {
namespace gfx {

class Screen {
 public:
  virtual ~Screen() = default;
  virtual void HandleInputs(const deskmate::input::InputState& input_state) = 0;
  virtual void Render(Display *display) const = 0;
};

}  // namespace deskmate
}  // namespace gfx

#endif  // DESKMATE_GFX_SCREEN_H