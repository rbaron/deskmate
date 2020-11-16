#include <iostream>
#include <memory>

#include "credentials.h"
#include "deskmate/gfx/components/text_list_item.h"
#include "deskmate/gfx/display.h"
#include "deskmate/gfx/screens/list.h"
#include "deskmate/gfx/screens/window.h"
#include "deskmate/input/input.h"
#include "deskmate/sdl2/gfx/display.h"
#include "deskmate/sdl2/input/input_transcriber.h"

constexpr unsigned int kDisplayHeight = 240;
constexpr unsigned int kDisplayWidth = 400;

using deskmate::gfx::Color;
using deskmate::gfx::Display;
using deskmate::gfx::Size;
using deskmate::gfx::components::TextListItem;
using deskmate::gfx::screens::ListItem;
using deskmate::gfx::screens::ListScreen;
using deskmate::gfx::screens::Window;
using deskmate::gfx::screens::WindowedScreen;
using deskmate::input::InputEvent;
using deskmate::sdl2::gfx::SDLDisplay;
using deskmate::sdl2::input::InputTranscriber;

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Could not initialize sdl2: " << SDL_GetError() << std::endl;
    return -1;
  }

  std::unique_ptr<Display> display =
      std::make_unique<SDLDisplay>(kDisplayHeight, kDisplayWidth);

  std::vector<std::unique_ptr<TextListItem>> text_list_items;
  for (int i = 0; i < 10; i++) {
    text_list_items.push_back(
        std::make_unique<TextListItem>("Hello " + std::to_string(i)));
  }
  std::vector<ListItem*> right_item_ptrs;
  for (const auto& list_item : text_list_items) {
    right_item_ptrs.push_back(list_item.get());
  }

  std::unique_ptr<ListScreen> list_screen_right =
      std::make_unique<ListScreen>(right_item_ptrs);

  const Size& size = display->GetSize();

  std::vector<WindowedScreen> windowed_screens = {
      {list_screen_right.get(),
       {{0, 0}, {display->GetSize().height, size.width / 2}},
       true},
      {list_screen_right.get(),
       {{0, size.width / 2}, {display->GetSize().height, size.width / 2}},
       true},
  };
  std::unique_ptr<Window> window = std::make_unique<Window>(windowed_screens);

  InputTranscriber input_transcriber(window.get());

  display->Clear();
  window->Render(display.get());
  display->Refresh();

  while (!input_transcriber.ShouldQuit()) {
    input_transcriber.DispatchEvents();
    display->Clear();
    window->Render(display.get());
    display->Refresh();
  }

  // TODO: fix destruction order. Right now the SDL_Quit is being called before
  // the destructor of SDLDisplay.
  SDL_Quit();

  return 0;
}