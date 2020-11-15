#include "deskmate/sdl2/gfx/display.h"

#include <SDL.h>

#include <iostream>
#include <memory>

namespace deskmate {
namespace sdl2 {
namespace gfx {
namespace {
using deskmate::gfx::Color;
using deskmate::gfx::Display;
using deskmate::gfx::Size;
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
  surface_ = SDL_GetWindowSurface(window_.get());
  if (surface_ == NULL) {
    std::cerr << "Unable to initialize surface\n";
    exit(-1);
  }
  SDL_FillRect(surface_, NULL,
               SDL_MapRGB(surface_->format, 0xFF, 0xFF, 0xFF));
  SDL_UpdateWindowSurface(window_.get());
  // SDL_Window* window = NULL;
  // SDL_Surface* screenSurface = NULL;

  // window = SDL_CreateWindow("hello_sdl2", SDL_WINDOWPOS_UNDEFINED,
  //                           SDL_WINDOWPOS_UNDEFINED, kDisplayWidth,
  //                           kDisplayHeight, SDL_WINDOW_SHOWN);
  // if (window == NULL) {
  //   fprintf(stderr, "could not create window: %s\n", SDL_GetError());
  //   return 1;
  // }
  // screenSurface = SDL_GetWindowSurface(window);
  // SDL_FillRect(screenSurface, NULL,
  //              SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
  // SDL_UpdateWindowSurface(window);
}

SDLDisplay::~SDLDisplay() {}

void SDLDisplay::Clear() {
  std::cerr << "window_: " << window_ << std::endl;
  std::cerr << "surface_: " << surface_ << std::endl;
  std::cerr << "1 surface_: " << surface_->format << std::endl;
  SDL_FillRect(surface_, NULL,
               SDL_MapRGB(surface_->format, 0xFF, 0xFF, 0xFF));
  std::cerr << "2 surface_: " << surface_ << std::endl;
}

void SDLDisplay::Refresh() { SDL_UpdateWindowSurface(window_.get()); }

void SDLDisplay::DrawPixelAbsolute(int y, int x, Color color) {}

void SDLDisplay::PutTextAbsolute(int y, int x, const std::string& text,
                                 int scale, Color fg, Color bg) {}

}  // namespace gfx
}  // namespace sdl2
}  // namespace deskmate