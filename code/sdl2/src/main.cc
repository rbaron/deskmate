#include <iostream>
#include <memory>

#include "credentials.h"
#include "deskmate/app/app.h"
#include "deskmate/sdl2/gfx/display.h"
#include "deskmate/sdl2/input/input_transcriber.h"
#include "deskmate/sdl2/net/mqtt.h"

using deskmate::app::App;
using deskmate::gfx::Display;
using deskmate::sdl2::gfx::SDLDisplay;
using deskmate::sdl2::input::InputTranscriber;
using deskmate::sdl2::net::PahoMQTTManager;

// Display.
using deskmate::credentials::kDisplayHeight;
using deskmate::credentials::kDisplayWidth;
using deskmate::credentials::kFontPath;

// MQTT.
using deskmate::credentials::kMQTTClientId;
using deskmate::credentials::kMQTTConfigs;
using deskmate::credentials::kMQTTPassword;
using deskmate::credentials::kMQTTPort;
using deskmate::credentials::kMQTTServer;
using deskmate::credentials::kMQTTUser;

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

int main(int argc, char *argv[]) {
  SDLIniter sdl_initer;

  PahoMQTTManager mqtt_manager(kMQTTServer, kMQTTPort, kMQTTUser, kMQTTPassword,
                               kMQTTClientId);

  if (!mqtt_manager.Connect()) {
    std::cerr << "Unable to connect to the MQTT server\n";
    std::exit(-1);
  }

  SDLDisplay display(kDisplayHeight, kDisplayWidth, kFontPath);

  App app(&display, &mqtt_manager);
  app.Init(kMQTTConfigs);

  InputTranscriber input_transcriber(app.GetInputEventHandler());

  while (!input_transcriber.ShouldQuit()) {
    input_transcriber.DispatchEvents();
    app.Tick();
  }
}