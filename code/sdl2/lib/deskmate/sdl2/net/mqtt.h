#ifndef DESKMATE_SDL2_NET_MQTT_H
#define DESKMATE_SDL2_NET_MQTT_H

#include <memory>

#include "MQTTClient.h"
#include "deskmate/mqtt/mqtt.h"

namespace deskmate {
namespace sdl2 {
namespace net {

namespace {
using deskmate::mqtt::MQTTMessage;
using deskmate::mqtt::MQTTMessageBuffer;
using deskmate::mqtt::MQTTMessageQueue;
}  // namespace

struct PahoMQTTClientDeleter {
  void operator()(MQTTClient* client) { MQTTClient_destroy(client); }
};

class PahoMQTTManager : public MQTTMessageBuffer {
 public:
  PahoMQTTManager(const char* server, int port, const char* username,
                  const char* password, const char* client_id);

  bool Connect() override;
  bool IsConnected() const override;

 private:
  bool SubscribeOnly(const std::string& topic) override;
  bool Publish(const MQTTMessage& msg) override;

  const std::string username_;
  const std::string password_;
  const std::string client_id_;
  std::unique_ptr<MQTTClient, PahoMQTTClientDeleter> mqtt_client_;
  MQTTClient_connectOptions conn_opts_;

  static int OnMessageReceived(void* context, char* topicName, int topicLen,
                               MQTTClient_message* message);
};

}  // namespace net
}  // namespace sdl2
}  // namespace deskmate

#endif  // DESKMATE_SDL2_NET_MQTT_H