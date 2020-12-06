#include "deskmate/arduino/net/mqtt.h"

#include <Arduino.h>
#include <WiFi.h>

#include <algorithm>

namespace deskmate {
namespace arduino {
namespace net {

namespace {
using deskmate::mqtt::MQTTMessage;
using deskmate::mqtt::MQTTSubscriber;

// Max that PubSubClient supports.
constexpr int kSubscriptionQoS = 1;
}  // namespace

MQTTManager::MQTTManager(const char* server, int port, const char* username,
                         const char* password, const char* client_id)
    : username_(username), password_(password), client_id_(client_id) {
  // Register the "On new message" callback, which calls Dispatch.
  // No fancy synchronization is needed here, since this callback only
  // runs when we call loop() in our main... loop. In other works, there
  // is no other thread.
  pubsub_client_ = std::make_unique<PubSubClient>(
      server, port,
      [this](const char* topic, byte* payload,  int length) {
        std::string str_payload =
            std::string(reinterpret_cast<char*>(payload), length);
        Serial.printf("[mqtt] Got message: %s -> %s\n", topic, str_payload.c_str());
        for (MQTTSubscriber* subs : this->subscribers_by_topic_[topic]) {
          subs->HandleMessage({topic, str_payload});
        }
      },
      wifi_client_);
}

bool MQTTManager::Connect() {
  Serial.println("[mqtt] Will connect.");
  return pubsub_client_->connect(client_id_.c_str(), username_.c_str(),
                                 password_.c_str());
}

bool MQTTManager::IsConnected() const { return pubsub_client_->connected(); }

bool MQTTManager::Subscribe(MQTTSubscriber* subscriber) {
  for (const std::string& topic : subscriber->SubscriptionTopics()) {
    subscribers_by_topic_[topic].push_back(subscriber);
    pubsub_client_->subscribe(topic.c_str(), kSubscriptionQoS);
  }
  return true;
}

// PubSubClient::publish only supports QoS = 0.
bool MQTTManager::Publish(const MQTTMessage& msg) {
  out_queue_.push(msg);
  return true;
}

bool MQTTManager::Tick() {
  pubsub_client_->loop();

  if (!IsConnected()) {
    Serial.println("[mqtt] Connection lost. Reconnecting.");
    if (!Connect()) {
      Serial.println("[mqtt] Unable to reconnect.");
      return false;
    }

    std::for_each(
        subscribers_by_topic_.cbegin(), subscribers_by_topic_.cend(),
        [this](
            const std::pair<std::string, std::vector<MQTTSubscriber*>>& pair) {
          return pubsub_client_->subscribe(pair.first.c_str(),
                                           kSubscriptionQoS);
        });
  }

  // Send out messages.
  while (!out_queue_.empty()) {
    const MQTTMessage& msg = out_queue_.front();
    if (!pubsub_client_->publish(msg.topic.c_str(), msg.payload.c_str())) {
      Serial.printf("[mqtt] Error sending message %s -> %s\n", msg.topic.c_str(), msg.payload.c_str());
    }
    out_queue_.pop();
  }

  return true;
}

}  // namespace net
}  // namespace arduino
}  // namespace deskmate
