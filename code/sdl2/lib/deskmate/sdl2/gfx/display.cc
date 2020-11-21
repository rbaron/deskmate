#include "deskmate/sdl2/gfx/display.h"

#include <iostream>
#include <memory>

#include "SDL.h"
#include "SDL2_gfxPrimitives.h"
#include "SDL_ttf.h"

namespace deskmate {
namespace sdl2 {
namespace gfx {
namespace {
using deskmate::gfx::Color;
using deskmate::gfx::Display;
using deskmate::gfx::Point;
using deskmate::gfx::Rect;
using deskmate::gfx::Size;

constexpr SDL_Color kSDLBlack{0x00, 0x00, 0x00, 0xff};
constexpr SDL_Color kSDLWhite{0xff, 0xff, 0xff, 0xff};

constexpr int kTTFFontInitSizePt = 36;

const SDL_Color& FromColor(Color color) {
  return color == Color::kBlack ? kSDLBlack : kSDLWhite;
}

void SetRenderDrawColor(SDL_Renderer* renderer, Color color) {
  const SDL_Color& sdl_color = FromColor(color);
  SDL_SetRenderDrawColor(renderer, sdl_color.r, sdl_color.g, sdl_color.b,
                         sdl_color.a);
}

SDL_Rect FromRect(const Rect& rect) {
  SDL_Rect sdl_rect;
  sdl_rect.x = rect.point.x;
  sdl_rect.y = rect.point.y;
  sdl_rect.w = rect.size.width;
  sdl_rect.h = rect.size.height;
  return sdl_rect;
}
}  // namespace

SDLDisplay::SDLDisplay(unsigned int height, unsigned int width,
                       const std::string& font_path)
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
      TTF_OpenFont(font_path.c_str(), kTTFFontInitSizePt));
  if (!font_) {
    std::cerr << "Unable to initialize SDL_ttf: " << TTF_GetError()
              << std::endl;
    exit(-1);
  }
}

SDLDisplay::~SDLDisplay() {}

void SDLDisplay::Clear() {
  SetRenderDrawColor(renderer_.get(), Color::kWhite);
  SDL_RenderClear(renderer_.get());
}

void SDLDisplay::Refresh() { SDL_RenderPresent(renderer_.get()); }

void SDLDisplay::DrawPixelAbsolute(int y, int x, Color color) {
  SetRenderDrawColor(renderer_.get(), color);
  SDL_RenderDrawPoint(renderer_.get(), x, y);
}

void SDLDisplay::PutTextAbsolute(int y, int x, const std::string& text,
                                 int scale, Color fg, Color bg) {
  const SDL_Color& text_color = fg == Color::kBlack ? kSDLBlack : kSDLWhite;

  // Draw text.
  // Low quality, fast.
  // auto text_surface = std::unique_ptr<SDL_Surface, SDLSurfaceDeleter>(
  //     TTF_RenderText_Solid(font_.get(), text.c_str(), text_color));
  // Mid quality, slow.
  // auto text_surface = std::unique_ptr<SDL_Surface, SDLSurfaceDeleter>(
  //     TTF_RenderText_Shaded(font_.get(), text.c_str(), text_color,
  //     back_color));
  // High quality, very slow.
  auto text_surface = std::unique_ptr<SDL_Surface, SDLSurfaceDeleter>(
      TTF_RenderText_Blended(font_.get(), text.c_str(), text_color));
  auto text_texture = std::unique_ptr<SDL_Texture, SDLTextureDeleter>(
      SDL_CreateTextureFromSurface(renderer_.get(), text_surface.get()));

  // Get texture size.
  SDL_Rect dest{x, y, static_cast<int>(kCharWidth * scale * text.length()),
                static_cast<int>(scale * kCharHeight)};
  // SDL_Rect dest{x, y, static_cast<int>(kCharWidth * scale * text.length())};
  // SDL_QueryTexture(text_texture.get(), nullptr, nullptr, &dest.w, &dest.h);

  // Draw background.
  SetRenderDrawColor(renderer_.get(), bg);
  SDL_RenderFillRect(renderer_.get(), &dest);

  // Render.
  SDL_RenderCopy(renderer_.get(), text_texture.get(), nullptr, &dest);
}

void SDLDisplay::DrawRectAbsolute(const Rect& rect, Color color) {
  SDL_Rect sdl_rect = FromRect(rect);
  SetRenderDrawColor(renderer_.get(), color);
  SDL_RenderDrawRect(renderer_.get(), &sdl_rect);
}

void SDLDisplay::FillRectAbsolute(const Rect& rect, Color color) {
  SDL_Rect sdl_rect = FromRect(rect);
  SetRenderDrawColor(renderer_.get(), color);
  SDL_RenderFillRect(renderer_.get(), &sdl_rect);
}

void SDLDisplay::DrawCircleAbsolute(Point center, unsigned int radius,
                                    Color color) {
  const SDL_Color& sdl_color = FromColor(color);
  circleRGBA(renderer_.get(), center.x, center.y, radius, sdl_color.r,
             sdl_color.g, sdl_color.b, sdl_color.a);
}

void SDLDisplay::FillCircleAbsolute(Point center, unsigned int radius,
                                    Color color) {
  const SDL_Color& sdl_color = FromColor(color);
  filledCircleRGBA(renderer_.get(), center.x, center.y, radius, sdl_color.r,
                   sdl_color.g, sdl_color.b, sdl_color.a);
}

}  // namespace gfx
}  // namespace sdl2
}  // namespace deskmate