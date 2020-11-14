#ifndef DESKMATE_ARDUINO_GFX_SHARP_MEM_DISPLAY_H
#define DESKMATE_ARDUINO_GFX_SHARP_MEM_DISPLAY_H

#include <Adafruit_SharpMem.h>

#include <memory>
#include <string>

#include "deskmate/gfx/display.h"

namespace deskmate {
namespace arduino {
namespace gfx {

namespace {

using deskmate::gfx::Color;
using deskmate::gfx::Size;

constexpr unsigned int kCharHeight = 8;
constexpr unsigned int kCharWidth = 6;

}  // namespace

class SharpMemDisplay : public deskmate::gfx::Display {
 public:
  SharpMemDisplay(unsigned int height, unsigned int width, uint8_t sck_pin,
                  uint8_t mosi_pin, uint8_t cs_pin);
  ~SharpMemDisplay() override;

  Size GetSize() const override { return {height_, width_}; };
  Size GetCharSize() const override { return {kCharHeight, kCharWidth}; };

  void Clear() override;
  void Refresh() override;
  void DrawPixel(int y, int x, Color color) override;
  void PutText(int y, int x, const std::string& text, int scale, Color fg,
               Color bg) override;

 private:
  const unsigned int height_;
  const unsigned int width_;
  std::unique_ptr<Adafruit_SharpMem> display_;
};

}  // namespace gfx
}  // namespace arduino
}  // namespace deskmate

#endif  // DESKMATE_ARDUINO_GFX_SHARP_MEM_DISPLAY_H