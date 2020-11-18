#ifndef DESKMATE_SDL_INPUT_INPUT_TRANSCRIBER_H
#define DESKMATE_SDL_INPUT_INPUT_TRANSCRIBER_H

#include "SDL.h"
#include "deskmate/input/input.h"

namespace deskmate {
namespace sdl2 {
namespace input {
namespace {
using deskmate::input::InputEventHandler;
}  // namespace

// Listens for SDL events and dispatches the transcribed event to
// the given InputEventHandler.
class InputTranscriber {
 public:
  InputTranscriber(InputEventHandler *handler) : handler_(handler) {}
  void DispatchEvents();
  bool ShouldQuit();
 private:
  InputEventHandler *handler_;
  SDL_Event event_;
  bool should_quit_ = false;

};
}  // namespace input
}  // namespace sdl2
}  // namespace deskmate
#endif  // DESKMATE_SDL_INPUT_INPUT_TRANSCRIBER_H