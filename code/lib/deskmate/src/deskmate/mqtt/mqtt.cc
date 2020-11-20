#include "deskmate/mqtt/mqtt.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <string>

namespace deskmate {
namespace mqtt {

bool MQTTMessageBuffer::Process() {
  if (!IsConnected()) {
    // std::cerr << "Not connected. Will reco\n";
    if (!Connect()) {
      return false;
    }
    // TODO: better error handling when re-subscribing.
    // Re-subscribe to topics.
    std::for_each(subscribed_topics_.cbegin(), subscribed_topics_.cend(),
                  [this](const std::string& topic) { Subscribe(topic); });
  }

  OnProcess();

  // Send messages.
  while (!out_queue_.empty()) {
    const MQTTMessage& msg = out_queue_.front();
    // std::cout << "Will send message: " << msg.topic << " -> " << msg.payload
    //           << )td::endl;
    if (Publish(msg)) {
      // std::cout << "Sent!\n";
    } else {
      // std::cout << "Failed!\n";
      return false;
    }
    out_queue_.pop();
  }
  return true;
}

bool MQTTMessageBuffer::Subscribe(const std::string& topic) {
  if (SubscribeOnly(topic)) {
    subscribed_topics_.push_back(topic);
    return true;
  }
  return false;
}

}  // namespace mqtt
}  // namespace deskmate
