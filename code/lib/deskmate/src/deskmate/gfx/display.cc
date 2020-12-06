#include "deskmate/gfx/display.h"

#include <algorithm>

#include "deskmate/gfx/display.h"
#include "deskmate/input/input.h"

namespace deskmate {
namespace gfx {

namespace {
using deskmate::gfx::Color;
using deskmate::gfx::Point;
using deskmate::gfx::Rect;
}  // namespace

Point operator+(Point lhs, const Point& rhs) {
  lhs += rhs;
  return lhs;
}

Point operator-(Point lhs, const Point& rhs) {
  lhs -= rhs;
  return lhs;
}

Display::Display(int height, int width) {
  windows_stack_.push(Rect{Point{0, 0}, Size{height, width}});
}

void Display::PushWindow(const Rect& window) {
  const Rect& current = windows_stack_.top();
  windows_stack_.push(Rect{current.point + window.point, window.size});
}

// TODO: die with a nice message if this is empty.
void Display::PopWindow() { windows_stack_.pop(); }

const Size& Display::GetSize() const { return windows_stack_.top().size; }

void Display::DrawPixel(int y, int x, Color color) {
  const Rect& window = windows_stack_.top();
  DrawPixelAbsolute(window.point.y + y, window.point.x + x, color);
}

void Display::DrawRect(Rect rect, Color color) {
  const Rect& window = windows_stack_.top();
  rect.point += window.point;
  DrawRectAbsolute(rect, color);
}

void Display::FillRect(Rect rect, Color color) {
  const Rect& window = windows_stack_.top();
  rect.point += window.point;
  FillRectAbsolute(rect, color);
}

void Display::DrawCircle(Point center, int radius, Color color) {
  const Rect& window = windows_stack_.top();
  center += window.point;
  DrawCircleAbsolute(center, radius, color);
}

void Display::FillCircle(Point center, int radius, Color color) {
  const Rect& window = windows_stack_.top();
  center += window.point;
  FillCircleAbsolute(center, radius, color);
}

void Display::PutText(int y, int x, const std::string& text, int scale,
                      Color fg, Color bg) {
  const Rect& window = windows_stack_.top();
  PutTextAbsolute(window.point.y + y, window.point.x + x, text, scale, fg, bg);
}
}  // namespace gfx
}  // namespace deskmate