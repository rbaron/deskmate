#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include <deque>
#include <string>
#include <vector>

#include "credentials.h"
#include "deskmate/arduino/gfx/sharp_mem_display.h"
#include "deskmate/arduino/input/buttons.h"
#include "deskmate/arduino/input/crank.h"
#include "deskmate/arduino/net/wifi.h"
#include "deskmate/gfx/screens/list.h"
#include "deskmate/input/input.h"

using deskmate::arduino::gfx::SharpMemDisplay;
using deskmate::arduino::input::SetupButtonsInterruptHandler;
using deskmate::arduino::input::SetupCrankInterruptHandler;
using deskmate::arduino::net::WiFiManager;
using deskmate::gfx::Display;
using deskmate::gfx::screens::ListItem;
using deskmate::gfx::screens::ListScreen;

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

struct MQTTMessage {
  std::string topic;
  std::string payload;
};
std::deque<MQTTMessage> mqtt_out_queue, mqtt_in_queue;

void MQTTCallback(const char* topic, byte* payload, unsigned int length) {
  Serial.println("Got message\n");
  mqtt_in_queue.push_back({topic, std::string((char*)payload, length)});
}

WiFiClient wificlient;
PubSubClient pubsubclient(deskmate::credentials::kMQTTServer,
                          deskmate::credentials::kMQTTPort, MQTTCallback,
                          wificlient);

Display* display = new SharpMemDisplay(kDisplayHeight, kDisplayWidth, kSCKPin,
                                       kMOSIPin, kCSPin);

class MQTTSubscriber {
 public:
  virtual ~MQTTSubscriber() = default;
  virtual std::string GetSubscriptionTopic() const = 0;

  void HandleMessage(const MQTTMessage& msg) {
    if (msg.topic == GetSubscriptionTopic()) {
      HandleOwnMessage(msg);
    }
  }

 private:
  virtual void HandleOwnMessage(const MQTTMessage& msg) = 0;
};

class MQTTListItem : public ListItem, public MQTTSubscriber {
 public:
  MQTTListItem(const std::string& display_name,
               const std::string& command_topic,
               const std::string& subscription_topic)
      : display_name_(display_name),
        command_topic_(command_topic),
        subscription_topic_(subscription_topic) {}

  std::string Render() const override {
    return display_name_ + (on_ ? " ON " : " OFF ");
  }

  void OnSelect() override {
    Serial.printf("%s Pressed!\n", display_name_.c_str());
    mqtt_out_queue.push_back({command_topic_, on_ ? "OFF" : "ON"});
  }

  std::string GetSubscriptionTopic() const override {
    return subscription_topic_;
  }

 private:
  void HandleOwnMessage(const MQTTMessage& msg) override {
    on_ = msg.payload == "ON";
  }

  std::string display_name_;
  std::string command_topic_;
  std::string subscription_topic_;
  bool on_ = false;
};

std::unique_ptr<ListScreen> list_screen;
std::vector<std::unique_ptr<MQTTListItem>> mqtt_list_items;

void setup() {
  for (const auto& cfg : deskmate::credentials::kMQTTConfigs) {
    mqtt_list_items.push_back(std::make_unique<MQTTListItem>(
        cfg.display_name, cfg.command_topic, cfg.state_topic));
  }

  // Construct
  std::vector<ListItem*> item_ptrs;
  for (const auto& list_item : mqtt_list_items) {
    item_ptrs.push_back(list_item.get());
  }
  list_screen = std::make_unique<ListScreen>(item_ptrs);

  SetupButtonsInterruptHandler(kCrankPushPin, kButtonAPin, kButtonBPin,
                               kButtonCPin, list_screen.get());
  SetupCrankInterruptHandler(kCrankAPin, kCrankBPin, list_screen.get());
  Serial.begin(9600);

  wifi_manager.Connect();
  pubsubclient.connect(deskmate::credentials::kMQTTClientId,
                       deskmate::credentials::kMQTTUser,
                       deskmate::credentials::kMQTTPassword);

  for (const auto& mqtt_list_item : mqtt_list_items) {
    pubsubclient.subscribe(mqtt_list_item->GetSubscriptionTopic().c_str());
  }
}

void loop() {
  auto before = millis();

  wifi_manager.MaybeReconnect();

  // TODO: maybe reconnect.
  pubsubclient.loop();

  // Process MQTT messages
  while (!mqtt_out_queue.empty()) {
    const MQTTMessage msg = mqtt_out_queue.front();
    mqtt_out_queue.pop_front();
    pubsubclient.publish(msg.topic.c_str(), msg.payload.c_str());
  }

  while (!mqtt_in_queue.empty()) {
    const MQTTMessage msg = mqtt_in_queue.front();
    mqtt_in_queue.pop_front();
    Serial.printf("Got msg: %s -> %s\n", msg.topic.c_str(),
                  msg.payload.c_str());
    for (const auto& mqtt_list_item : mqtt_list_items) {
      mqtt_list_item->HandleMessage(msg);
    }
  }

  list_screen->Render(display);
  delay(10);
  // Serial.println();
}