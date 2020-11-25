#include "deskmate/gfx/components/mqtt_circle_horizontal_list_item.h"

#include "deskmate/gfx/display.h"
#include "deskmate/mqtt/mqtt.h"

namespace deskmate {
namespace gfx {
namespace components {
namespace {
using deskmate::gfx::components::CircleHorizontalListItem;
using deskmate::mqtt::MQTTSubscriber;
}  // namespace

MQTTCircleHorizontalListItem::MQTTCircleHorizontalListItem(
    const std::string& display_name, const std::string& unit,
    const std::string& value_topic, const std::string& availability_topic)
    : CircleHorizontalListItem(display_name, unit, /*value=*/0,
                               /*is_available=*/false),
      value_topic_(value_topic),
      availability_topic_(availability_topic) {}

void MQTTCircleHorizontalListItem::HandleMessage(
    const deskmate::mqtt::MQTTMessage& msg) {
  if (msg.topic == value_topic_) {
    value_ = std::stod(msg.payload);
  } else if (msg.topic == availability_topic_) {
    is_available_ = msg.payload == "online";
  }
}

const std::vector<std::string>
MQTTCircleHorizontalListItem::SubscriptionTopics() const {
  return {value_topic_, availability_topic_};
}

}  // namespace components
}  // namespace gfx
}  // namespace deskmate