#include "deskmate/mqtt/mqtt.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <string>

namespace deskmate {
namespace mqtt {

bool MQTTMessageBuffer::ProcessInner() {
  if (!IsConnected()) {
    if (!Connect()) {
      return false;
    }
    // Re-subscribe to topics.
    // TODO: better error handling when re-subscribing.
    std::for_each(
        subscribers_by_topic_.cbegin(), subscribers_by_topic_.cend(),
        [this](
            const std::pair<std::string, std::vector<MQTTSubscriber*>>& pair) {
          std::for_each(
              pair.second.cbegin(), pair.second.cend(),
              [this](MQTTSubscriber* subscriber) { Subscribe(subscriber); });
        });
  }

  // Send messages.
  while (!out_queue_.empty()) {
    const MQTTMessage& msg = out_queue_.front();
    if (!Publish(msg)) {
      return false;
    }
    out_queue_.pop();
  }
  return true;
}

bool MQTTMessageBuffer::Subscribe(MQTTSubscriber* subscriber) {
  const std::string& topic = subscriber->GetSubscriptionTopic();
  if (SubscribeOnly(topic)) {
    subscribers_by_topic_[topic].push_back(subscriber);
    return true;
  }
  return false;
}

bool MQTTMessageBuffer::Dispatch(const MQTTMessage& msg) {
  if (subscribers_by_topic_.count(msg.topic) > 0) {
    for (MQTTSubscriber* subscriber : subscribers_by_topic_[msg.topic]) {
      subscriber->HandleMessage(msg);
    }
  }
  return true;
}

}  // namespace mqtt
}  // namespace deskmate
