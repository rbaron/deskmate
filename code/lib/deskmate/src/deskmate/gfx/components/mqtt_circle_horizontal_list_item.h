#ifndef DESKMATE_GFX_COMPONENTS_MQTT_CIRCLE_HORIZONTAL_LIST_ITEM_H
#define DESKMATE_GFX_COMPONENTS_MQTT_CIRCLE_HORIZONTAL_LIST_ITEM_H

#include "deskmate/gfx/components/circle_horizontal_list_item.h"
#include "deskmate/gfx/display.h"
#include "deskmate/mqtt/mqtt.h"

namespace deskmate {
namespace gfx {
namespace components {

class MQTTCircleHorizontalListItem
    : public deskmate::gfx::components::CircleHorizontalListItem,
      public deskmate::mqtt::MQTTSubscriber {
 public:
  MQTTCircleHorizontalListItem(const std::string& display_name,
                               const std::string& unit,
                               const std::string& value_topic,
                               const std::string& availability_topic);

  void HandleMessage(const deskmate::mqtt::MQTTMessage& msg) override;

  const std::vector<std::string> SubscriptionTopics() const override;

 private:
  const std::string value_topic_;
  const std::string availability_topic_;
};

}  // namespace components
}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_COMPONENTS_MQTT_CIRCLE_HORIZONTAL_LIST_ITEM_H