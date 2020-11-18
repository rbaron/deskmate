#ifndef DESKMATE_GFX_DISPLAY_H
#define DESKMATE_GFX_DISPLAY_H

#include <stack>
#include <string>

namespace deskmate {
namespace gfx {

enum class Color {
  kBlack,
  kWhite,
};

struct Point {
  unsigned int y;
  unsigned int x;
};

struct Size {
  unsigned int height;
  unsigned int width;
};

struct Rect {
  // Upper left corner.
  Point point;
  Size size;
};

// Display is an interface for LCD/OLED/Software-based displays.
class Display {
 public:
  Display(unsigned int height, unsigned int width);
  virtual ~Display() = default;

  // Changes the "apparent" drawable area. These are convenience functions for
  // rendering nested views. Pushing a window will make the apparent
  // dimensions of the display take that value temporarily, and when components
  // render themselves, they only know the "current" dimensions and should
  // position themselves accordingly.
  void PushWindow(const Rect& window);
  void PopWindow();

  // Get the display size in pixels. This might be the "apparent" size (based on
  // the sub-windowss that have been pushed via PushWindow).
  const Size& GetSize() const;

  // Returns the Size of a single char. For now we assume monospace fonts.
  virtual Size GetCharSize() const = 0;
  virtual void Clear() = 0;
  virtual void Refresh() = 0;

  void DrawPixel(int y, int x, Color color);

  void PutText(int y, int x, const std::string& text, int scale, Color fg,
               Color bg);

  void PutText(int y, int x, const std::string& text, int scale, Color fg) {
    return PutText(y, x, text, scale, fg, Color::kWhite);
  };

 private:
  // Derived classes must implement PutTextAbsolute and DrawPixelAbsolute, which
  // handle absolute coordinates. This base class exposes public PutText and
  // DrawPixel, for which the y and x coordinates are taken relative to the
  // current window (top of the windows_stack_).
  virtual void PutTextAbsolute(int y, int x, const std::string& text, int scale,
                               Color fg, Color bg) = 0;

  virtual void DrawPixelAbsolute(int y, int x, Color color) = 0;

  std::stack<Rect> windows_stack_;
};

}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_DISPLAY_H