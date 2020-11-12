#ifndef DESKMATE_GFX_DISPLAY_H
#define DESKMATE_GFX_DISPLAY_H

#include <string>

namespace deskmate {
namespace gfx {

class Display {
 public:
  virtual ~Display() = default;
  virtual unsigned int Height() = 0;
  virtual unsigned int Width() = 0;
  virtual void Clear() = 0;
  virtual void Refresh() = 0;
  virtual void DrawPixel(int y, int x, bool white) = 0;
  virtual void PutText(int y, int x, const std::string& text, int size,
                       bool white_fg, bool white_bg) = 0;
  void PutText(int y, int x, const std::string& text, int size, bool white) {
    return PutText(y, x, text, size, /*white_fg=*/white, /*white_bg=*/false);
  };
};

}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_DISPLAY_H