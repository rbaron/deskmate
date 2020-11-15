#include "deskmate/arduino/gfx/sharp_mem_display.h"

#include <Adafruit_SharpMem.h>

#include <memory>
#include <string>

namespace deskmate {
namespace arduino {
namespace gfx {

namespace {

using deskmate::gfx::Color;
using deskmate::gfx::Size;

constexpr int ColorToInt(Color color) { return color == Color::kBlack ? 0 : 1; }

}  // namespace

SharpMemDisplay::SharpMemDisplay(unsigned int height, unsigned int width,
                                 uint8_t sck_pin, uint8_t mosi_pin,
                                 uint8_t cs_pin)
    : height_(height),
      width_(width),
      display_(std::make_unique<Adafruit_SharpMem>(sck_pin, mosi_pin, cs_pin,
                                                   width, height)),
      window_({Point{0, 0}, Size{height, width}}) {
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

void SharpMemDisplay::DrawPixel(int y, int x, Color color) {
  display_->drawPixel(x + window_.point.x, y + window_.point.y, ColorToInt(color));
}

void SharpMemDisplay::PutText(int y, int x, const std::string& text, int scale,
                              Color fg, Color bg) {
  display_->setTextColor(ColorToInt(fg), ColorToInt(bg));
  display_->setTextSize(scale);
  display_->setCursor(x + window_.point.x, y + window_.point.y);
  display_->write(text.c_str());
}

}  // namespace gfx
}  // namespace arduino
}  // namespace deskmate