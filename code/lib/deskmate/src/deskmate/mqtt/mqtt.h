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
  virtual bool Connect() = 0;
  virtual bool IsConnected() const = 0;

  // Subscribes to a topic and remembers it so it automatically reconnects
  // when the connection is restablished.
  bool Subscribe(const std::string& topic);

  // Process does the following things:
  // 1. Ensures the client is connected (may reconnect and resubscribe if not)
  // 2. Pushes all received messages into the input queue.
  // 2. Publishes all outgoing messages from the output queue.
  bool Process();
  MQTTMessageQueue* InputQueue() { return &in_queue_; }
  MQTTMessageQueue* OutputQueue() { return &out_queue_; }

 private:
  // Simply handles a subscription. Do not care about re-subscribing.
  virtual bool SubscribeOnly(const std::string& topic) = 0;

  // Not to be called directly. Will be called by Process.
  virtual bool Publish(const MQTTMessage& msg) = 0;
  // Stores subscribed topics so we can re-subscribe upon reconnection, if the
  // connection drop.
  std::vector<std::string> subscribed_topics_;

  MQTTMessageQueue in_queue_;
  MQTTMessageQueue out_queue_;
};

}  // namespace mqtt
}  // namespace deskmate

#endif  // DESKMATE_INPUT_MQTT_H