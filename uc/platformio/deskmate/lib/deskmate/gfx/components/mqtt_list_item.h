#ifndef DESKMATE_GFX_COMPONENTS_MQTT_LIST_ITEM_H
#define DESKMATE_GFX_COMPONENTS_MQTT_LIST_ITEM_H

#include "deskmate/gfx/screens/list.h"
#include "deskmate/mqtt/mqtt.h"

namespace deskmate {
namespace gfx {
namespace components {

namespace {
using deskmate::gfx::screens::ListItem;
using deskmate::mqtt::MQTTMessage;
using deskmate::mqtt::MQTTSubscriber;
using deskmate::mqtt::MQTTMessageQueue;
}  // namespace

// Represents a ListItem that issues MQTT messages when clicked and listens and
// reacts to a topic.
class MQTTListItem : public ListItem, public MQTTSubscriber {
 public:
  MQTTListItem(const std::string& display_name,
               const std::string& command_topic,
               const std::string& subscription_topic,
               MQTTMessageQueue* mqtt_out_queue)
      : display_name_(display_name),
        command_topic_(command_topic),
        subscription_topic_(subscription_topic),
        mqtt_out_queue_(mqtt_out_queue) {}

  std::string Render() const override {
    return display_name_ + (on_ ? " ON " : " OFF ");
  }

  void OnSelect() override {
    // Serial.printf("%s Pressed!\n", display_name_.c_str());
    mqtt_out_queue_->push({command_topic_, on_ ? "OFF" : "ON"});
  }

  std::string GetSubscriptionTopic() const override {
    return subscription_topic_;
  }

 private:
  void HandleOwnMessage(const MQTTMessage& msg) override {
    on_ = msg.payload == "ON";
  }

  std::string display_name_;
  std::string command_topic_;
  std::string subscription_topic_;
  MQTTMessageQueue* mqtt_out_queue_;
  bool on_ = false;
};

}  // namespace components
}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_COMPONENTS_MQTT_LIST_ITEM_H