#ifndef DESKMATE_GFX_COMPONENTS_MQTT_LIST_ITEM_H
#define DESKMATE_GFX_COMPONENTS_MQTT_LIST_ITEM_H

#include "deskmate/gfx/display.h"
#include "deskmate/gfx/screens/list.h"
#include "deskmate/mqtt/mqtt.h"

namespace deskmate {
namespace gfx {
namespace components {

namespace {
using deskmate::gfx::Color;
using deskmate::gfx::Display;
using deskmate::gfx::Size;
using deskmate::gfx::screens::ListItem;
using deskmate::mqtt::MQTTMessage;
using deskmate::mqtt::MQTTMessageQueue;
using deskmate::mqtt::MQTTSubscriber;
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

  void Render(Display* display, bool is_selected) const override {
    const unsigned int char_scale = 2;
    const Size& size = display->GetSize();
    const Size& char_size = display->GetCharSize();
    std::string text = display_name_ + (on_ ? " ON " : " OFF ");
    if (text.length() * char_scale * char_size.width > size.width) {
      text = text.substr(0, size.width / (char_scale * char_size.width) - 1);
      text += ".";
    }
    display->PutText(0, 0, text, char_scale,
                     is_selected ? Color::kWhite : Color::kBlack,
                     is_selected ? Color::kBlack : Color::kWhite);
  }

  void OnSelect() override {
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