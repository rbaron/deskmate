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
  void operator()(MQTTClient *client) { MQTTClient_destroy(client); }
};

class PahoMQTTManager : public MQTTMessageBuffer {
 public:
  PahoMQTTManager(const char* server, int port, const char* username,
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
  std::unique_ptr<MQTTClient, PahoMQTTClientDeleter> mqtt_client_;
  MQTTClient_connectOptions conn_opts_;// = MQTTClient_connectOptions_initializer;

  // Stores subscribed topics so we can re-subscribe upon reconnection, if the
  // connection drop.
  std::vector<std::string> subscribed_topics_;

  static int OnMessageReceived(void *context, char *topicName, int topicLen, MQTTClient_message *message);
};

}  // namespace net
}  // namespace sdl2
}  // namespace deskmate

#endif  // DESKMATE_SDL2_NET_MQTT_H