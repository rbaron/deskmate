#ifndef DESKMATE_SDL2_NET_MQTT_H
#define DESKMATE_SDL2_NET_MQTT_H

#include <memory>
#include <mutex>
#include <queue>
#include <shared_mutex>

// Temporarily disable compiler error checking on third-party header.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "MQTTClient.h"
#pragma GCC diagnostic pop

#include "deskmate/mqtt/mqtt.h"

namespace deskmate {
namespace sdl2 {
namespace net {

struct PahoMQTTClientDeleter {
  void operator()(MQTTClient* client) { MQTTClient_destroy(client); }
};

class PahoMQTTManager : public deskmate::mqtt::MQTTMessageBuffer {
 public:
  PahoMQTTManager(const char* server, int port, const char* username,
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
  std::unique_ptr<MQTTClient, PahoMQTTClientDeleter> mqtt_client_;
  MQTTClient_connectOptions conn_opts_;

  // Mutex for synchronizing read/write access to in_queue_. On the
  // main thread, we dispatch incoming messages in this queue to their
  // respective MQTTSubscribers. On a different thread, the paho library will
  // populate this queue via our registered callback OnMessageReceived.
  std::shared_mutex input_queue_mutex_;

  // Holds pointers to subscribers so it can re-subscribe when the connection
  // drops. Pointers are expected to outlive instances of this class.
  std::unordered_map<std::string, std::vector<deskmate::mqtt::MQTTSubscriber*>>
      subscribers_by_topic_;

  // Holds incomming messages to be dispatched to subscribers in the main loop.
  std::queue<deskmate::mqtt::MQTTMessage> in_queue_;

  // Static callback. void* context will point to an instance of this class.
  static int OnMessageReceived(void* context, char* topicName, int topicLen,
                               MQTTClient_message* message);
};

}  // namespace net
}  // namespace sdl2
}  // namespace deskmate

#endif  // DESKMATE_SDL2_NET_MQTT_H