#include "deskmate/gfx/screens/horizontal_list.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "credentials.h"
#include "deskmate/gfx/components/circle_horizontal_list_item.h"
#include "deskmate/gfx/display.h"
#include "deskmate/sdl2/gfx/display.h"
#include "deskmate/sdl2/input/input_transcriber.h"

using deskmate::gfx::Color;
using deskmate::gfx::Display;
using deskmate::gfx::Point;
using deskmate::gfx::Rect;
using deskmate::gfx::Size;
using deskmate::gfx::components::CircleHorizontalListItem;
using deskmate::gfx::screens::HorizontalList;
using deskmate::gfx::screens::HorizontalListItem;
using deskmate::sdl2::gfx::SDLDisplay;
using deskmate::sdl2::input::InputTranscriber;

// Display.
using deskmate::credentials::kDisplayHeight;
using deskmate::credentials::kDisplayWidth;
using deskmate::credentials::kFontPath;

constexpr unsigned int kItemWidth = 90;

class SDLIniter {
 public:
  SDLIniter() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      std::cerr << "Could not initialize sdl2: " << SDL_GetError() << std::endl;
      std::exit(-1);
    }
    std::cout << "SDL initialized\n";

    // TODO: rendered stuff looks blurry.
    // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
  }
  ~SDLIniter() { SDL_Quit(); }
};

int main(int argc, char* argv[]) {
  std::srand(0);

  SDLIniter sdl_initer;
  SDLDisplay display(kDisplayHeight, kDisplayWidth, kFontPath);

  std::vector<std::unique_ptr<HorizontalListItem>> items;
  for (int i = 0; i < 15; i++) {
    items.push_back(std::make_unique<CircleHorizontalListItem>(
        "Item #" + std::to_string(i), "°C",
        /*value=*/std::rand() % 100 - 50,
        /*is_available=*/rand() % 2 == 0));
  }

  HorizontalList screen(items, kItemWidth);

  InputTranscriber input_transcriber(&screen);

  while (!input_transcriber.ShouldQuit()) {
    display.Clear();
    screen.Render(&display);
    display.Refresh();

    input_transcriber.DispatchEvents();
  }

  return 0;
}