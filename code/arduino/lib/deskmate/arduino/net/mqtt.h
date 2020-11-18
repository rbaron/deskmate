#ifndef DESKMATE_ARDUINO_NET_MQTT_H
#define DESKMATE_ARDUINO_NET_MQTT_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include <memory>

#include "deskmate/mqtt/mqtt.h"

namespace deskmate {
namespace arduino {
namespace net {

namespace {
using deskmate::mqtt::MQTTMessage;
using deskmate::mqtt::MQTTMessageBuffer;
using deskmate::mqtt::MQTTMessageQueue;
}  // namespace

class MQTTManager : public MQTTMessageBuffer {
 public:
  MQTTManager(const char* server, int port, const char* username,
              const char* password, const char* client_id);

  bool Connect();

  MQTTMessageQueue* InputQueue() override { return &in_queue_; }

  MQTTMessageQueue* OutputQueue() override { return &out_queue_; }

  bool Subscribe(const std::string& topic);

  // Ensures connected, sends messages from out_queue_ and receives messages
  // into in_queue_.
  bool Process();

 private:
  const std::string username_;
  const std::string password_;
  const std::string client_id_;
  MQTTMessageQueue in_queue_;
  MQTTMessageQueue out_queue_;
  WiFiClient wifi_client_;
  std::unique_ptr<PubSubClient> pubsub_client_;

  // Stores subscribed topics so we can re-subscribe upon reconnection, if the
  // connection drop.
  std::vector<std::string> subscribed_topics_;
};

}  // namespace net
}  // namespace arduino
}  // namespace deskmate

#endif  // DESKMATE_ARDUINO_NET_MQTT_H