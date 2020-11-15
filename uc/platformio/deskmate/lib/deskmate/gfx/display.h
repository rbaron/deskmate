#ifndef DESKMATE_GFX_DISPLAY_H
#define DESKMATE_GFX_DISPLAY_H

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

class Display {
 public:
  virtual ~Display() = default;
  // Get display size in pixels.
  virtual Size GetSize() const = 0;
  // Returns the Size of a single char. For now we assume monospace fonts.
  virtual Size GetCharSize() const = 0;

  // Changes the "apparent" drawable area.
  virtual void SetWindow(const Rect& window) = 0;

  virtual void Clear() = 0;
  virtual void Refresh() = 0;
  virtual void DrawPixel(int y, int x, Color color) = 0;
  virtual void PutText(int y, int x, const std::string& text, int scale,
                       Color fg, Color bg) = 0;
  void PutText(int y, int x, const std::string& text, int scale, Color fg) {
    return PutText(y, x, text, scale, fg, Color::kWhite);
  };
};

}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_DISPLAY_H