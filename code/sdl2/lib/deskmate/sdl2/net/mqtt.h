#ifndef DESKMATE_SDL2_NET_MQTT_H
#define DESKMATE_SDL2_NET_MQTT_H

#include <memory>
#include <mutex>

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

// The paho MQTTClient releases the mutex before calling the registered
// callback:
// https://github.com/eclipse/paho.mqtt.c/blob/v1.3.6/src/MQTTClient.c#L856
// So, theoretically, if MQTTClient runs on several threads, the callbacks
// could potentially be called concurrently. From a quick glance at the code,
// it seems like there is a single Thread_start call for which the receiving
// callback is triggered:
// (https://github.com/eclipse/paho.mqtt.c/blob/v1.3.6/src/MQTTClient.c#L1176),
// So, still, if the paho callback inserts a received message in our
// input_queue, I think no two inserts will be done at the same time.
// BUT, we would still need to coordinate _our_ access to the input_queue for
// when we want to dispatch the messages to our MQTTSubscribers.
// I considered two options:
// 1. Make the callback "synchronous" by having the callback call the
// MQTTSubscriber::HandleOwnMessage, with no concept of an input queue. This
// still needs syncrhonization, since the callback is called from another
// thread.
// 2. Use locks in our code to synchronize access to the input queue between
// the paho callback and our code.
// I currenctly implement approach #1, as it was easier to implement on the
// derived class only (single-threaded derived classes don't need to care about
// it.).

// There is a MQTTAsync version of the paho library, which is "thread safe":
// https://www.eclipse.org/paho/files/mqttdoc/MQTTAsync/html/index.html
// But the thread safety in this case refers to _our_ program running on
// multiple threads and calling the same MQTTAsync client. This is not a
// concern for now, as our program is single-threaded.
class PahoMQTTManager : public MQTTMessageBuffer {
 public:
  PahoMQTTManager(const char* server, int port, const char* username,
                  const char* password, const char* client_id);

  bool Connect() override;
  bool IsConnected() const override;
  bool Process() override;

  bool EnqueueForSending(const MQTTMessage& msg) override;

 private:
  bool SubscribeOnly(const std::string& topic) override;
  bool Publish(const MQTTMessage& msg) override;

  const std::string username_;
  const std::string password_;
  const std::string client_id_;
  std::unique_ptr<MQTTClient, PahoMQTTClientDeleter> mqtt_client_;
  MQTTClient_connectOptions conn_opts_;

  std::mutex lock_;

  static int OnMessageReceived(void* context, char* topicName, int topicLen,
                               MQTTClient_message* message);
};

}  // namespace net
}  // namespace sdl2
}  // namespace deskmate

#endif  // DESKMATE_SDL2_NET_MQTT_H