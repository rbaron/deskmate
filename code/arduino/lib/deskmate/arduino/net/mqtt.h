#ifndef DESKMATE_ARDUINO_NET_MQTT_H
#define DESKMATE_ARDUINO_NET_MQTT_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include <memory>
#include <queue>

#include "deskmate/mqtt/mqtt.h"

namespace deskmate {
namespace arduino {
namespace net {

// The PubSubClient reads network packets only upon calling the loop() function:
// https://github.com/knolleary/pubsubclient/blob/v2.8/src/PubSubClient.cpp#L389
// This means we don't need to coordinate access to the base class input queue,
// since this library is mean for single-thread environments.
class MQTTManager : public deskmate::mqtt::MQTTMessageBuffer {
 public:
  MQTTManager(const char* server, int port, const char* username,
              const char* password, const char* client_id);

  bool Connect() override;
  bool IsConnected() const override;
  bool Subscribe(deskmate::mqtt::MQTTSubscriber* subscriber) override;
  bool Publish(const deskmate::mqtt::MQTTMessage& msg) override;
  bool Tick() override;

 private:
  const std::string username_;
  const std::string password_;
  const std::string client_id_;
  WiFiClient wifi_client_;
  std::unique_ptr<PubSubClient> pubsub_client_;

  // Holds pointers to subscribers so it can re-subscribe when the connection
  // drops. Pointers are expected to outlive instances of this class.
  std::unordered_map<std::string, std::vector<deskmate::mqtt::MQTTSubscriber*>>
      subscribers_by_topic_;

  // Holds messages to be published in the main loop. These messages were likely
  // put here from interrupt service routines (for example when a button is
  // pushed).
  std::queue<deskmate::mqtt::MQTTMessage> out_queue_;
};

}  // namespace net
}  // namespace arduino
}  // namespace deskmate

#endif  // DESKMATE_ARDUINO_NET_MQTT_H