
#include <iostream>
#include <memory>
#include <vector>

#include "credentials.h"
#include "deskmate/gfx/components/text_list_item.h"
#include "deskmate/gfx/screens/list.h"
#include "deskmate/sdl2/gfx/display.h"
#include "deskmate/sdl2/input/input_transcriber.h"

using deskmate::gfx::Display;
using deskmate::gfx::components::TextListItem;
using deskmate::gfx::screens::ListItem;
using deskmate::gfx::screens::ListScreen;
using deskmate::sdl2::gfx::SDLDisplay;
using deskmate::sdl2::input::InputTranscriber;

// Display.
using deskmate::credentials::kDisplayHeight;
using deskmate::credentials::kDisplayWidth;
using deskmate::credentials::kFontPath;

class SDLIniter {
 public:
  SDLIniter() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      std::cerr << "Could not initialize sdl2: " << SDL_GetError() << std::endl;
      std::exit(-1);
    }
    std::cout << "SDL initialized\n";
  }
  ~SDLIniter() { SDL_Quit(); }
};

int main() {
  SDLIniter sdl_initer;
  SDLDisplay display(kDisplayHeight, kDisplayWidth, kFontPath);

  std::vector<std::unique_ptr<ListItem>> items;
  for (int i = 0; i < 20; i++) {
    items.push_back(
        std::make_unique<TextListItem>("Item #" + std::to_string(i)));
  }

  ListScreen screen(items);

  InputTranscriber input_transcriber(&screen);

  while (!input_transcriber.ShouldQuit()) {
    display.Clear();
    screen.Render(&display);
    display.Refresh();

    input_transcriber.DispatchEvents();
  }

  return 0;
}