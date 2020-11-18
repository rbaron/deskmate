#include <iostream>
#include <memory>

#include "MQTTClient.h"
#include "credentials_sdl2.h"
#include "deskmate/gfx/components/mqtt_list_item.h"
#include "deskmate/gfx/components/text_list_item.h"
#include "deskmate/gfx/display.h"
#include "deskmate/gfx/screens/list.h"
#include "deskmate/gfx/screens/window.h"
#include "deskmate/input/input.h"
#include "deskmate/sdl2/gfx/display.h"
#include "deskmate/sdl2/input/input_transcriber.h"
#include "deskmate/sdl2/net/mqtt.h"

constexpr unsigned int kDisplayHeight = 240;
constexpr unsigned int kDisplayWidth = 400;

using deskmate::gfx::Color;
using deskmate::gfx::Display;
using deskmate::gfx::Size;
using deskmate::gfx::components::MQTTListItem;
using deskmate::gfx::components::TextListItem;
using deskmate::gfx::screens::ListItem;
using deskmate::gfx::screens::ListScreen;
using deskmate::gfx::screens::Window;
using deskmate::gfx::screens::WindowedScreen;
using deskmate::input::InputEvent;
using deskmate::mqtt::MQTTMessage;
using deskmate::mqtt::MQTTSubscriber;
using deskmate::sdl2::gfx::SDLDisplay;
using deskmate::sdl2::input::InputTranscriber;
using deskmate::sdl2::net::PahoMQTTManager;

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Could not initialize sdl2: " << SDL_GetError() << std::endl;
    return -1;
  }

  // Make sure to use a different client_id for each program connecting to your
  // MQTT server at the same time, otherwise the connection will be dropped at
  // weird times and you'll keep bumping dropping each other out.
  PahoMQTTManager mqtt_manager(
      deskmate::credentials::kMQTTServer, deskmate::credentials::kMQTTPort,
      deskmate::credentials::kMQTTUser, deskmate::credentials::kMQTTPassword,
      deskmate::credentials::kMQTTClientId);

  mqtt_manager.Connect();

  std::unique_ptr<Display> display =
      std::make_unique<SDLDisplay>(kDisplayHeight, kDisplayWidth, deskmate::credentials::kFontPath);

  // Left split screen.
  std::vector<std::unique_ptr<MQTTListItem>> mqtt_list_items;
  std::vector<ListItem *> left_item_ptrs;
  for (const auto &cfg : deskmate::credentials::kMQTTConfigs) {
    mqtt_list_items.push_back(std::make_unique<MQTTListItem>(
        cfg.display_name, cfg.command_topic, cfg.state_topic,
        mqtt_manager.OutputQueue()));
    left_item_ptrs.push_back(mqtt_list_items.back().get());
    std::cout << "Will subs to: " << cfg.state_topic << std::endl;
    mqtt_manager.Subscribe(cfg.state_topic);
  }

  std::unique_ptr<ListScreen> list_screen_left =
      std::make_unique<ListScreen>(left_item_ptrs);

  // Right split screen.
  std::vector<std::unique_ptr<TextListItem>> text_list_items;
  std::vector<ListItem *> right_item_ptrs;
  for (int i = 0; i < 10; i++) {
    text_list_items.push_back(
        std::make_unique<TextListItem>("Hello " + std::to_string(i)));
    right_item_ptrs.push_back(text_list_items.back().get());
  }
  std::unique_ptr<ListScreen> list_screen_right =
      std::make_unique<ListScreen>(right_item_ptrs);

  const Size &size = display->GetSize();

  std::vector<WindowedScreen> windowed_screens = {
      {list_screen_left.get(),
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
    mqtt_manager.Process();
    auto *mqtt_in_queue = mqtt_manager.InputQueue();
    while (!mqtt_in_queue->empty()) {
      const MQTTMessage &msg = mqtt_in_queue->front();
      std::cout << "Got message: " << msg.topic << " -> " << msg.payload
                << std::endl;
      for (const auto &mqtt_list_item : mqtt_list_items) {
        mqtt_list_item->HandleMessage(msg);
      }
      mqtt_in_queue->pop();
    }
    display->Clear();
    window->Render(display.get());
    display->Refresh();
  }

  // TODO: fix destruction order. Right now the SDL_Quit is being called before
  // the destructor of SDLDisplay.
  SDL_Quit();

  return 0;
}