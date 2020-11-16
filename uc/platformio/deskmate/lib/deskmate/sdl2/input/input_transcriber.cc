#include "deskmate/sdl2/input/input_transcriber.h"

#include "SDL.h"

namespace deskmate {
namespace sdl2 {
namespace input {
namespace {
using deskmate::input::InputEvent;

InputEvent Transcribe(SDL_Keysym keysym) {
  switch (keysym.sym) {
    case SDLK_a:
      return InputEvent::kAPush;
    case SDLK_b:
      return InputEvent::kBPush;
    case SDLK_c:
      return InputEvent::kCPush;
    case SDLK_RIGHT:
      return InputEvent::kCrankCW;
    case SDLK_LEFT:
      return InputEvent::kCrankCCW;
    default:
      return InputEvent::kUnknown;
  }
}

}  // namespace

void InputTranscriber::DispatchEvents() {
  while (SDL_PollEvent(&event_)) {
    if (event_.type == SDL_KEYDOWN) {
      handler_->HandleInputEvent(Transcribe(event_.key.keysym));
    } else if (event_.type == SDL_QUIT) {
      should_quit_ = true;
    }
  }
}

bool InputTranscriber::ShouldQuit() {
  return should_quit_;
}

}  // namespace input
}  // namespace sdl2
}  // namespace deskmate