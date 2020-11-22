#ifndef DESKMATE_GFX_COMPONENTS_MQTT_LIST_ITEM_H
#define DESKMATE_GFX_COMPONENTS_MQTT_LIST_ITEM_H

#include "deskmate/gfx/display.h"
#include "deskmate/gfx/screens/list.h"
#include "deskmate/mqtt/mqtt.h"

namespace deskmate {
namespace gfx {
namespace components {

// Represents a ListItem that issues MQTT messages when clicked and listens and
// reacts to a topic.
class MQTTListItem : public deskmate::gfx::screens::ListItem,
                     public deskmate::mqtt::MQTTSubscriber {
 public:
  MQTTListItem(const std::string& display_name,
               const std::string& command_topic, const std::string& state_topic,
               deskmate::mqtt::MQTTMessageBuffer* mqtt_buffer);

  void Render(deskmate::gfx::Display* display, bool is_selected) const override;

  void OnSelect() override;

  void HandleMessage(const deskmate::mqtt::MQTTMessage& msg) override;

  const std::vector<std::string> SubscriptionTopics() const override;

 private:
  std::string display_name_;
  std::string command_topic_;
  std::string state_topic_;
  deskmate::mqtt::MQTTMessageBuffer* mqtt_buffer_;
  bool on_ = false;
};

}  // namespace components
}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_COMPONENTS_MQTT_LIST_ITEM_H