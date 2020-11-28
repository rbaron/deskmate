#ifndef DESKMATE_INPUT_MQTT_H
#define DESKMATE_INPUT_MQTT_H

#include <queue>
#include <string>
#include <unordered_map>

namespace deskmate {
namespace mqtt {

struct MQTTMessage {
  std::string topic;
  std::string payload;
};

class MQTTSubscriber {
 public:
  virtual ~MQTTSubscriber() = default;
  virtual void HandleMessage(const MQTTMessage& msg) = 0;
  virtual const std::vector<std::string> SubscriptionTopics() const = 0;
};

// Interface for providing queues for inbound and outbound messages.
class MQTTMessageBuffer {
 public:
  virtual bool Connect() = 0;
  virtual bool IsConnected() const = 0;
  virtual bool Subscribe(MQTTSubscriber* subscriber) = 0;
  virtual bool Publish(const MQTTMessage& msg) = 0;

  // This will be called once every main app tick.
  virtual bool Tick() { return true; };
};

}  // namespace mqtt
}  // namespace deskmate

#endif  // DESKMATE_INPUT_MQTT_H