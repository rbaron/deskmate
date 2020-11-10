#ifndef DESKMATE_ARDUINO_GFX_SHARP_MEM_DISPLAY_H
#define DESKMATE_ARDUINO_GFX_SHARP_MEM_DISPLAY_H

#include <memory>
#include <string>

#include <Adafruit_SharpMem.h>

#include "deskmate/gfx/display.h"

namespace deskmate {
namespace arduino {
namespace gfx {

class SharpMemDisplay : public deskmate::gfx::Display {
public:
  SharpMemDisplay(unsigned int height, unsigned int width, uint8_t sck_pin, uint8_t mosi_pin, uint8_t cs_pin);
  unsigned int Height() { return height_; }
  unsigned int Width() { return width_; }
  virtual ~SharpMemDisplay() override;
  void Clear() override;
  void Refresh() override;
  void DrawPixel(int y, int x, bool white) override;
  void PutText(int y, int x, const std::string& text, int size, bool white) override;
private:
  const unsigned int height_;
  const unsigned int width_;
  std::unique_ptr<Adafruit_SharpMem> display_;
};

}  // namespace gfx
}  // namespace arduino
}  // namespace deskmate

#endif  // DESKMATE_ARDUINO_GFX_SHARP_MEM_DISPLAY_H