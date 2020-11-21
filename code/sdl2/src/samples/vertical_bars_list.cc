#include "deskmate/gfx/screens/vertical_bars_list.h"

#include <iostream>
#include <memory>
#include <vector>

#include "credentials.h"
#include "deskmate/sdl2/gfx/display.h"
#include "deskmate/sdl2/input/input_transcriber.h"

using deskmate::gfx::screens::VerticalBarListItem;
using deskmate::gfx::screens::VerticalBarsList;
using deskmate::sdl2::gfx::SDLDisplay;
using deskmate::sdl2::input::InputTranscriber;

// Display.
using deskmate::credentials::kDisplayHeight;
using deskmate::credentials::kDisplayWidth;
using deskmate::credentials::kFontPath;

// Testing only.
class FakeBarItem : public VerticalBarListItem {
 public:
  FakeBarItem(const std::string& display_name, double percentage,
              bool is_filled)
      : display_name_(display_name),
        percentage_(percentage),
        is_filled_(is_filled) {}
  double Percentage() const override { return percentage_; }
  bool IsFilled() const override { return is_filled_; }
  const std::string& DisplayName() const override { return display_name_; }

 private:
  const std::string display_name_;
  double percentage_;
  bool is_filled_;
};

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
  SDLIniter sdl_initer;
  SDLDisplay display(kDisplayHeight, kDisplayWidth, kFontPath);

  std::vector<std::unique_ptr<VerticalBarListItem>> items;
  items.push_back(std::make_unique<FakeBarItem>("Calethea", 0.3723, true));
  items.push_back(std::make_unique<FakeBarItem>("Ficus", 0.972, true));
  items.push_back(std::make_unique<FakeBarItem>("Citrus", 0.05, true));
  items.push_back(std::make_unique<FakeBarItem>("Pilea", 0.55, false));

  VerticalBarsList screen(items);

  InputTranscriber input_transcriber(&screen);

  while (!input_transcriber.ShouldQuit()) {
    display.Clear();
    screen.Render(&display);
    display.Refresh();

    input_transcriber.DispatchEvents();
  }

  return 0;
}