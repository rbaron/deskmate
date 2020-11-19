#include "deskmate/arduino/net/mqtt.h"

#include <Arduino.h>
#include <WiFi.h>

#include <algorithm>

namespace deskmate {
namespace arduino {
namespace net {

namespace {
using deskmate::mqtt::MQTTMessage;
}  // namespace

MQTTManager::MQTTManager(const char* server, int port, const char* username,
                         const char* password, const char* client_id)
    : username_(username), password_(password), client_id_(client_id) {
  // Register the "On new message" callback, which puts the message into
  // this->InputQueue().
  pubsub_client_ = std::make_unique<PubSubClient>(
      server, port,
      [this](const char* topic, byte* payload, unsigned int length) {
        std::string str_payload =
            std::string(reinterpret_cast<char*>(payload), length);
        Serial.printf("Got mqtt message %s -> %s\n", topic,
                      str_payload.c_str());
        this->InputQueue()->push({topic, str_payload});
      },
      wifi_client_);
}

bool MQTTManager::Connect() {
  return pubsub_client_->connect(client_id_.c_str(), username_.c_str(),
                                 password_.c_str());
}

bool MQTTManager::IsConnected() const { return pubsub_client_->connected(); }

bool MQTTManager::SubscribeOnly(const std::string& topic) {
  return pubsub_client_->subscribe(topic.c_str());
}
bool MQTTManager::Publish(const MQTTMessage& msg) {
  return pubsub_client_->publish(msg.topic.c_str(), msg.payload.c_str());
}

}  // namespace net
}  // namespace arduino
}  // namespace deskmate
