#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include <queue>
#include <string>
#include <vector>

#include "credentials.h"
#include "deskmate/arduino/gfx/sharp_mem_display.h"
#include "deskmate/arduino/input/buttons.h"
#include "deskmate/arduino/input/crank.h"
#include "deskmate/arduino/net/mqtt.h"
#include "deskmate/arduino/net/wifi.h"
#include "deskmate/gfx/components/mqtt_list_item.h"
#include "deskmate/gfx/screens/list.h"
#include "deskmate/input/input.h"
#include "deskmate/mqtt/mqtt.h"

using deskmate::arduino::gfx::SharpMemDisplay;
using deskmate::arduino::input::SetupButtonsInterruptHandler;
using deskmate::arduino::input::SetupCrankInterruptHandler;
using deskmate::arduino::net::MQTTManager;
using deskmate::arduino::net::WiFiManager;
using deskmate::gfx::Color;
using deskmate::gfx::Display;
using deskmate::gfx::components::MQTTListItem;
using deskmate::gfx::screens::ListItem;
using deskmate::gfx::screens::ListScreen;
using deskmate::mqtt::MQTTMessage;
using deskmate::mqtt::MQTTSubscriber;

// Input pins.
constexpr uint8_t kButtonAPin = 27;
constexpr uint8_t kButtonBPin = 26;
constexpr uint8_t kButtonCPin = 25;
constexpr uint8_t kCrankAPin = 33;
constexpr uint8_t kCrankBPin = 34;
constexpr uint8_t kCrankPushPin = 32;

// Display pins.
constexpr uint8_t kSCKPin = 14;
constexpr uint8_t kMOSIPin = 13;
constexpr uint8_t kCSPin = 15;

// Display dimensions.
constexpr unsigned int kDisplayHeight = 240;
constexpr unsigned int kDisplayWidth = 400;

WiFiManager wifi_manager(deskmate::credentials::kWIFISSID,
                         deskmate::credentials::kWIFIPass);

MQTTManager mqtt_manager(deskmate::credentials::kMQTTServer,
                         deskmate::credentials::kMQTTPort,
                         deskmate::credentials::kMQTTUser,
                         deskmate::credentials::kMQTTPassword,
                         deskmate::credentials::kMQTTClientId);

Display* display = new SharpMemDisplay(kDisplayHeight, kDisplayWidth, kSCKPin,
                                       kMOSIPin, kCSPin);

std::unique_ptr<ListScreen> list_screen;
std::vector<std::unique_ptr<MQTTListItem>> mqtt_list_items;

void setup() {
  for (const auto& cfg : deskmate::credentials::kMQTTConfigs) {
    mqtt_list_items.push_back(std::make_unique<MQTTListItem>(
        cfg.display_name, cfg.command_topic, cfg.state_topic,
        mqtt_manager.OutputQueue()));
  }

  std::vector<ListItem*> item_ptrs;
  for (const auto& list_item : mqtt_list_items) {
    item_ptrs.push_back(list_item.get());
  }
  list_screen = std::make_unique<ListScreen>(item_ptrs);

  SetupButtonsInterruptHandler(kCrankPushPin, kButtonAPin, kButtonBPin,
                               kButtonCPin, list_screen.get());
  SetupCrankInterruptHandler(kCrankAPin, kCrankBPin, list_screen.get());

  Serial.begin(9600);

  display->Clear();
  display->PutText(10, 10, "Connecting to WiFi...", 1, Color::kBlack);
  display->Refresh();
  Serial.println("Will connect to wifi.");
  bool wifi_conn = wifi_manager.Connect();
  Serial.printf("Connected to wifi: %d\n.", wifi_conn);

  display->Clear();
  display->PutText(20, 10, "Connecting to MQTT broker...", 1, Color::kBlack);
  Serial.println("Will connect to pubsub.");
  bool mqtt_conn = mqtt_manager.Connect();
  Serial.printf("Connected to pubsub: %d\n.", mqtt_conn);

  for (const auto& mqtt_list_item : mqtt_list_items) {
    mqtt_manager.Subscribe(mqtt_list_item->GetSubscriptionTopic());
  }
}

void loop() {
  auto before = millis();

  wifi_manager.MaybeReconnect();

  mqtt_manager.Process();

  auto* mqtt_in_queue = mqtt_manager.InputQueue();
  while (!mqtt_in_queue->empty()) {
    const MQTTMessage& msg = mqtt_in_queue->front();
    Serial.printf("Got msg: %s -> %s\n", msg.topic.c_str(),
                  msg.payload.c_str());
    for (const auto& mqtt_list_item : mqtt_list_items) {
      mqtt_list_item->HandleMessage(msg);
    }
    mqtt_in_queue->pop();
  }

  list_screen->Render(display);
  delay(10);
  // Serial.println();
}