#include "deskmate/sdl2/gfx/display.h"

#include <iostream>
#include <memory>

#include "SDL.h"
#include "SDL_ttf.h"

namespace deskmate {
namespace sdl2 {
namespace gfx {
namespace {
using deskmate::gfx::Color;
using deskmate::gfx::Display;
using deskmate::gfx::Size;

constexpr SDL_Color kSDLBlack{0x00, 0x00, 0x00, 0xff};
constexpr SDL_Color kSDLWhite{0xff, 0xff, 0xff, 0xff};
}  // namespace

SDLDisplay::SDLDisplay(unsigned int height, unsigned int width)
    : Display(height, width) {
  window_ = std::unique_ptr<SDL_Window, SDLWindowDeleter>(SDL_CreateWindow(
      "deskmate", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,
      height, SDL_WINDOW_SHOWN));
  if (window_.get() == NULL) {
    std::cerr << "Unable to initialize window\n";
    exit(-1);
  }
  renderer_ = std::unique_ptr<SDL_Renderer, SDLRendererDeleter>(
      SDL_CreateRenderer(window_.get(), /*index=*/-1, /*flags=*/0));
  if (!renderer_) {
    std::cerr << "Unable to initialize renderer\n";
    exit(-1);
  }
  if (TTF_Init() < 0) {
    std::cerr << "Unable to initialize SDL_ttf\n";
    exit(-1);
  }
  font_ = std::unique_ptr<TTF_Font, SDLFontDeleter>(
      TTF_OpenFont("/Users/rbaron/dev/deskmate/uc/platformio/deskmate/assets/"
                   "fonts/DejaVu/DejaVuSansMono.ttf",
                   12));
  if (!font_) {
    std::cerr << "Unable to initialize SDL_ttf: " << TTF_GetError()
              << std::endl;
    exit(-1);
  }
}

SDLDisplay::~SDLDisplay() {}

void SDLDisplay::Clear() {
  SDL_SetRenderDrawColor(renderer_.get(), 0xff, 0xff, 0xff, 0xff);
  SDL_RenderClear(renderer_.get());
}

void SDLDisplay::Refresh() { SDL_RenderPresent(renderer_.get()); }

void SDLDisplay::DrawPixelAbsolute(int y, int x, Color color) {
  SDL_SetRenderDrawColor(renderer_.get(), 0, 0, 0, 0xff);
  SDL_RenderDrawPoint(renderer_.get(), x, y);
}

void SDLDisplay::PutTextAbsolute(int y, int x, const std::string& text,
                                 int scale, Color fg, Color bg) {
  const SDL_Color& text_color = fg == Color::kBlack ? kSDLBlack : kSDLWhite;
  const SDL_Color& back_color = bg == Color::kBlack ? kSDLBlack : kSDLWhite;

  SDL_Rect dest{x, y, static_cast<int>(kCharWidth * scale * text.length()),
                static_cast<int>(scale * kCharHeight)};

  // Draw background.
  SDL_SetRenderDrawColor(renderer_.get(), back_color.r, back_color.g,
                         back_color.b, back_color.a);
  SDL_RenderFillRect(renderer_.get(), &dest);

  // Draw text.
  auto text_surface = std::unique_ptr<SDL_Surface, SDLSurfaceDeleter>(
      TTF_RenderText_Solid(font_.get(), text.c_str(), text_color));
  auto text_texture = std::unique_ptr<SDL_Texture, SDLTextureDeleter>(
      SDL_CreateTextureFromSurface(renderer_.get(), text_surface.get()));

  SDL_RenderCopy(renderer_.get(), text_texture.get(), nullptr, &dest);
}

}  // namespace gfx
}  // namespace sdl2
}  // namespace deskmate