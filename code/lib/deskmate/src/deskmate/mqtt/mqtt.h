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

  // Derived classes can override this to implement locking/synchronization,
  // if they run in multiple thread.
  virtual bool Process() { return ProcessInner(); };

  // Subscribes to a topic and remembers it so it automatically reconnects
  // when the connection is restablished.
  bool Subscribe(MQTTSubscriber* subscriber);

  MQTTMessageQueue* OutputQueue() { return &out_queue_; }

 protected:
  // Sends the received message to registered callbacks. If the derived class
  // do multi-threaded stuff, it is up to them to synchronize this call in
  // relatinship to the Process function.
  bool Dispatch(const MQTTMessage& msg);

  // ProcessInner does the following:
  // 1. Ensures the client is connected (may reconnect and resubscribe if not)
  // 2. Publishes all outgoing messages from the output queue.
  bool ProcessInner();

 private:
  // Simply handles a subscription. Do not care about re-subscribing.
  virtual bool SubscribeOnly(const std::string& topic) = 0;

  // Not to be called directly. Will be called by Process.
  virtual bool Publish(const MQTTMessage& msg) = 0;

  // Stores subscriptions so we can re-subscribe upon reconnection if the
  // connection drop.
  std::unordered_map<std::string, std::vector<MQTTSubscriber*>>
      subscribers_by_topic_;

  MQTTMessageQueue out_queue_;
};

}  // namespace mqtt
}  // namespace deskmate

#endif  // DESKMATE_INPUT_MQTT_H