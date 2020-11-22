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

// The PubSubClient reads network packets only upon calling the loop() function:
// https://github.com/knolleary/pubsubclient/blob/v2.8/src/PubSubClient.cpp#L389
// This means we don't need to coordinate access to the base class input queue:
// We can let loop() populate the input queue and later we can dispatch the input-
// enqueued messages to their registered MQTTSubscribers.
class MQTTManager : public MQTTMessageBuffer {
 public:
  MQTTManager(const char* server, int port, const char* username,
              const char* password, const char* client_id);

  bool Connect() override;
  bool IsConnected() const override;
  bool Process() override;

 private:
  // Simply handles a subscription. Do not care about re-subscribing.
  bool SubscribeOnly(const std::string& topic) override;

  // Not to be called directly. Will be called by Process.
  bool Publish(const MQTTMessage& msg) override;

  const std::string username_;
  const std::string password_;
  const std::string client_id_;
  WiFiClient wifi_client_;
  std::unique_ptr<PubSubClient> pubsub_client_;
};

}  // namespace net
}  // namespace arduino
}  // namespace deskmate

#endif  // DESKMATE_ARDUINO_NET_MQTT_H