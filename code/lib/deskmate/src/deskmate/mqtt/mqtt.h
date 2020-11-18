#ifndef DESKMATE_INPUT_MQTT_H
#define DESKMATE_INPUT_MQTT_H

#include <queue>
#include <string>

namespace deskmate {
namespace mqtt {

struct MQTTMessage {
  std::string topic;
  std::string payload;
};

class MQTTSubscriber {
 public:
  virtual ~MQTTSubscriber() = default;
  virtual std::string GetSubscriptionTopic() const = 0;

  void HandleMessage(const MQTTMessage& msg) {
    if (msg.topic == GetSubscriptionTopic()) {
      HandleOwnMessage(msg);
    }
  }

 private:
  virtual void HandleOwnMessage(const MQTTMessage& msg) = 0;
};

using MQTTMessageQueue = std::queue<MQTTMessage>;

// Interface for providing queues for inbound and outbound messages.
class MQTTMessageBuffer {
 public:
  virtual MQTTMessageQueue* InputQueue() = 0;
  virtual MQTTMessageQueue* OutputQueue() = 0;
};

}  // namespace mqtt
}  // namespace deskmate

#endif  // DESKMATE_INPUT_MQTT_H