#ifndef DESKMATE_SDL_GFX_DISPLAY_H
#define DESKMATE_SDL_GFX_DISPLAY_H

#include <memory>

#include "SDL.h"
#include "SDL_ttf.h"
#include "deskmate/gfx/display.h"

namespace deskmate {
namespace sdl2 {
namespace gfx {
namespace {
using deskmate::gfx::Color;
using deskmate::gfx::Display;
using deskmate::gfx::Size;

// Not really.
constexpr int kCharHeight = 8;
constexpr int kCharWidth = 6;
}  // namespace

struct SDLRendererDeleter {
  void operator()(SDL_Renderer* renderer) { SDL_DestroyRenderer(renderer); }
};

struct SDLWindowDeleter {
  void operator()(SDL_Window* window) { SDL_DestroyWindow(window); }
};

struct SDLTextureDeleter {
  void operator()(SDL_Texture* texture) { SDL_DestroyTexture(texture); }
};

struct SDLSurfaceDeleter {
  void operator()(SDL_Surface* surface) { SDL_FreeSurface(surface); }
};

struct SDLFontDeleter {
  void operator()(TTF_Font* font) { TTF_CloseFont(font); }
};

class SDLDisplay : public Display {
 public:
  // font_path is the filesystem path to a monospace font.
  SDLDisplay(int height, int width, const std::string& font_path);
  ~SDLDisplay() override;

  Size GetCharSize() const override { return {kCharHeight, kCharWidth}; };

  void Clear() override;
  void Refresh() override;

 private:
  void PutTextAbsolute(int y, int x, const std::string& text, int scale,
                       Color fg, Color bg) override;
  void DrawPixelAbsolute(int y, int x, Color color) override;
  void DrawRectAbsolute(const deskmate::gfx::Rect& rect,
                        deskmate::gfx::Color color) override;
  void FillRectAbsolute(const deskmate::gfx::Rect& rect,
                        deskmate::gfx::Color color) override;
  void DrawCircleAbsolute(deskmate::gfx::Point center, int radius,
                          deskmate::gfx::Color color) override;
  void FillCircleAbsolute(deskmate::gfx::Point center, int radius,
                          deskmate::gfx::Color color) override;

  std::unique_ptr<TTF_Font, SDLFontDeleter> font_;
  std::unique_ptr<SDL_Renderer, SDLRendererDeleter> renderer_;
  std::unique_ptr<SDL_Window, SDLWindowDeleter> window_;
};

}  // namespace gfx
}  // namespace sdl2
}  // namespace deskmate
#endif  // DESKMATE_SDL_GFX_DISPLAY_H