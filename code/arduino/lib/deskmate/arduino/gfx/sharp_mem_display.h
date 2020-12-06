#ifndef DESKMATE_ARDUINO_GFX_SHARP_MEM_DISPLAY_H
#define DESKMATE_ARDUINO_GFX_SHARP_MEM_DISPLAY_H

#include <Adafruit_SharpMem.h>

#include <memory>
#include <stack>
#include <string>

#include "deskmate/gfx/display.h"

namespace deskmate {
namespace arduino {
namespace gfx {

namespace {
constexpr int kCharHeight = 8;
constexpr int kCharWidth = 6;
}  // namespace

class SharpMemDisplay : public deskmate::gfx::Display {
 public:
  SharpMemDisplay(int height, int width, uint8_t sck_pin, uint8_t mosi_pin,
                  uint8_t cs_pin);
  ~SharpMemDisplay() override;

  deskmate::gfx::Size GetCharSize() const override {
    return {kCharHeight, kCharWidth};
  };

  void Clear() override;
  void Refresh() override;

 private:
  void DrawPixelAbsolute(int y, int x, deskmate::gfx::Color color) override;

  void PutTextAbsolute(int y, int x, const std::string& text, int scale,
                       deskmate::gfx::Color fg,
                       deskmate::gfx::Color bg) override;

  void DrawRectAbsolute(const deskmate::gfx::Rect& rect,
                        deskmate::gfx::Color color) override;
  void FillRectAbsolute(const deskmate::gfx::Rect& rect,
                        deskmate::gfx::Color color) override;
  void DrawCircleAbsolute(deskmate::gfx::Point center, int radius,
                          deskmate::gfx::Color color) override;
  void FillCircleAbsolute(deskmate::gfx::Point center, int radius,
                          deskmate::gfx::Color color) override;

  std::unique_ptr<Adafruit_SharpMem> display_;
};

}  // namespace gfx
}  // namespace arduino
}  // namespace deskmate

#endif  // DESKMATE_ARDUINO_GFX_SHARP_MEM_DISPLAY_H