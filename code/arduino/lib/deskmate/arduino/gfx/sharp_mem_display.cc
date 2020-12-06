#include "deskmate/arduino/gfx/sharp_mem_display.h"

#include <Adafruit_SharpMem.h>

#include <memory>
#include <string>

namespace deskmate {
namespace arduino {
namespace gfx {

namespace {

using deskmate::gfx::Color;
using deskmate::gfx::Rect;
using deskmate::gfx::Size;

constexpr int ColorToInt(Color color) { return color == Color::kBlack ? 0 : 1; }

}  // namespace

SharpMemDisplay::SharpMemDisplay(int height, int width, uint8_t sck_pin,
                                 uint8_t mosi_pin, uint8_t cs_pin)
    : Display(height, width),
      display_(std::make_unique<Adafruit_SharpMem>(sck_pin, mosi_pin, cs_pin,
                                                   width, height)) {
  display_->begin();
  display_->clearDisplay();
  display_->cp437(true);
}

SharpMemDisplay::~SharpMemDisplay() {}

void SharpMemDisplay::Clear() {
  // Clears the actual display's content, via SPI.
  // display_->clearDisplay();

  // Clears the content of the in-RAM buffer. Since the RAM buffer
  // will be flushed to the display via SPI with the Refresh() call,
  // omiting this round-trip saves us from flickering.
  display_->clearDisplayBuffer();
}

void SharpMemDisplay::Refresh() { display_->refresh(); }

void SharpMemDisplay::DrawPixelAbsolute(int y, int x, Color color) {
  display_->drawPixel(x, y, ColorToInt(color));
}

void SharpMemDisplay::PutTextAbsolute(int y, int x, const std::string& text,
                                      int scale, Color fg, Color bg) {
  display_->setTextColor(ColorToInt(fg), ColorToInt(bg));
  display_->setTextSize(scale);
  display_->setCursor(x, y);
  display_->write(text.c_str());
}

void SharpMemDisplay::DrawRectAbsolute(const deskmate::gfx::Rect& rect,
                                       deskmate::gfx::Color color) {
  display_->drawRect(rect.point.x, rect.point.y, rect.size.width,
                     rect.size.height, ColorToInt(color));
}

void SharpMemDisplay::FillRectAbsolute(const deskmate::gfx::Rect& rect,
                                       deskmate::gfx::Color color) {
  display_->fillRect(rect.point.x, rect.point.y, rect.size.width,
                     rect.size.height, ColorToInt(color));
}

void SharpMemDisplay::DrawCircleAbsolute(deskmate::gfx::Point center,
                                         int radius,
                                         deskmate::gfx::Color color) {
  display_->drawCircle(center.x, center.y, radius, ColorToInt(color));
}

void SharpMemDisplay::FillCircleAbsolute(deskmate::gfx::Point center,
                                         int radius,
                                         deskmate::gfx::Color color) {
  display_->fillCircle(center.x, center.y, radius, ColorToInt(color));
}

}  // namespace gfx
}  // namespace arduino
}  // namespace deskmate