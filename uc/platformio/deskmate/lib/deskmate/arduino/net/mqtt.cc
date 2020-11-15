#include "deskmate/arduino/net/mqtt.h"

#include <Arduino.h>
#include <WiFi.h>

namespace deskmate {
namespace arduino {
namespace net {

namespace {}  // namespace

MQTTManager::MQTTManager(const char* server, int port, const char* username,
                         const char* password, const char* client_id)
    : username_(username), password_(password), client_id_(client_id) {
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

bool MQTTManager::Subscribe(const std::string& topic) {
  return pubsub_client_->subscribe(topic.c_str());
}

bool MQTTManager::Process() {
  if (!pubsub_client_->connected()) {
    // TODO: apparently when reconnecting we loose the subscriptions?
    // I think we need to re-subscribe. This is done in this example:
    // https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_reconnect_nonblocking/mqtt_reconnect_nonblocking.ino
    Serial.println("MQTTManager is not connected. Connecting.");
    Connect();
  } else {
    pubsub_client_->loop();
  }

  // TODO: Better error handling.
  while (!out_queue_.empty()) {
    const MQTTMessage& msg = out_queue_.front();
    Serial.printf("Will send message %s -> %s\n", msg.topic.c_str(),
                  msg.payload.c_str());
    if (pubsub_client_->publish(msg.topic.c_str(), msg.payload.c_str())) {
      Serial.println("Sent!");
    } else {
      Serial.println("Failed!");
    }
    out_queue_.pop();
  }
  return true;
}

}  // namespace net
}  // namespace arduino
}  // namespace deskmate
