#ifndef DESKMATE_SDL_GFX_DISPLAY_H
#define DESKMATE_SDL_GFX_DISPLAY_H

#include <SDL.h>

#include "deskmate/gfx/display.h"

namespace deskmate {
namespace sdl2 {
namespace gfx {
namespace {
using deskmate::gfx::Color;
using deskmate::gfx::Display;
using deskmate::gfx::Size;

// Not really.
constexpr unsigned int kCharHeight = 8;
constexpr unsigned int kCharWidth = 6;
}  // namespace

struct SDLWindowDeleter {
  void operator()(SDL_Window* window) { SDL_DestroyWindow(window); }
};

class SDLDisplay : public Display {
 public:
  SDLDisplay(unsigned int height, unsigned int width);
  ~SDLDisplay() override;

  Size GetCharSize() const override { return {kCharHeight, kCharWidth}; };

  void Clear() override;
  void Refresh() override;

 private:
  void DrawPixelAbsolute(int y, int x, Color color) override;

  void PutTextAbsolute(int y, int x, const std::string& text, int scale,
                       Color fg, Color bg) override;

  std::unique_ptr<SDL_Window, SDLWindowDeleter> window_;

  // window_ will free this pointer when destroyed.
  SDL_Surface *surface_;
};

}  // namespace gfx
}  // namespace sdl2
}  // namespace deskmate
#endif  // DESKMATE_SDL_GFX_DISPLAY_H