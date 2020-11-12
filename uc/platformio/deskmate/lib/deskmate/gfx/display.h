#ifndef DESKMATE_GFX_DISPLAY_H
#define DESKMATE_GFX_DISPLAY_H

#include <string>

namespace deskmate {
namespace gfx {

enum class Color {
  kBlack,
  kWhite,
};

class Display {
 public:
  virtual ~Display() = default;
  virtual unsigned int Height() = 0;
  virtual unsigned int Width() = 0;
  virtual void Clear() = 0;
  virtual void Refresh() = 0;
  virtual void DrawPixel(int y, int x, Color color) = 0;
  virtual void PutText(int y, int x, const std::string& text, int size,
                       Color fg, Color bg) = 0;
  void PutText(int y, int x, const std::string& text, int size, Color fg) {
    return PutText(y, x, text, size, fg, Color::kWhite);
  };
};

}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_DISPLAY_H