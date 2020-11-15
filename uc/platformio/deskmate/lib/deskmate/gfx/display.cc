#include "deskmate/gfx/display.h"

#include <Arduino.h>

#include <algorithm>

#include "deskmate/gfx/display.h"
#include "deskmate/input/input.h"

namespace deskmate {
namespace gfx {

namespace {
using deskmate::gfx::Color;
using deskmate::gfx::Rect;
}  // namespace

Display::Display(unsigned int height, unsigned int width) {
  windows_stack_.push(Rect{Point{0, 0}, Size{height, width}});
}

void Display::PushWindow(const Rect& window) {
  const Rect& current = windows_stack_.top();
  windows_stack_.push(Rect{
      Point{current.point.y + window.point.y, current.point.x + window.point.x},
      window.size});
}

// TODO: die with a nice message if this is empty.
void Display::PopWindow() { windows_stack_.pop(); }

const Size& Display::GetSize() const { return windows_stack_.top().size; }

void Display::DrawPixel(int y, int x, Color color) {
  const Rect& window = windows_stack_.top();
  DrawPixelAbsolute(window.point.y + y, window.point.x + x, color);
}

void Display::PutText(int y, int x, const std::string& text, int scale,
                              Color fg, Color bg) {
  const Rect& window = windows_stack_.top();
  PutTextAbsolute(window.point.y + y, window.point.x + x, text, scale, fg, bg);
}

}  // namespace gfx
}  // namespace deskmate