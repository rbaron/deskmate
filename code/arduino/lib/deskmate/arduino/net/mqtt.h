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

  virtual bool Connect() override;
  virtual bool IsConnected() const override;

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