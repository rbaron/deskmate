#include <iostream>
#include <memory>

#include "deskmate/gfx/display.h"
#include "deskmate/gfx/screens/list.h"
#include "deskmate/sdl2/gfx/display.h"

constexpr unsigned int kDisplayHeight = 480;
constexpr unsigned int kDisplayWidth = 640;

using deskmate::gfx::Display;
using deskmate::sdl2::gfx::SDLDisplay;

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Could not initialize sdl2: " << SDL_GetError() << std::endl;
    return -1;
  }

  std::unique_ptr<Display> display =
      std::make_unique<SDLDisplay>(kDisplayHeight, kDisplayWidth);

  display->Clear();
  display->Refresh();

  SDL_Event e;
  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
      if (e.type == SDL_KEYDOWN) {
        quit = true;
      }
      if (e.type == SDL_MOUSEBUTTONDOWN) {
        quit = true;
      }
    }
  }

  // TODO: fix destruction order. Right now the SDL_Quit is being called before
  // the destructor of SDLDisplay.
  SDL_Quit();

  return 0;
}