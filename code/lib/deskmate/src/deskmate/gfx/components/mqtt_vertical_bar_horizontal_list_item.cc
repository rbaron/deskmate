#include "deskmate/gfx/components/mqtt_vertical_bar_horizontal_list_item.h"

#include "deskmate/gfx/display.h"
#include "deskmate/mqtt/mqtt.h"

namespace deskmate {
namespace gfx {
namespace components {
namespace {
using deskmate::gfx::components::VerticalBarHorizontalListItem;
using deskmate::mqtt::MQTTSubscriber;
}  // namespace

MQTTVerticalBarHorizontalListItem::MQTTVerticalBarHorizontalListItem(
    const std::string& display_name, const std::string& value_topic,
    const std::string& availability_topic)
    : VerticalBarHorizontalListItem(display_name, /*percentage=*/0.0,
                                    /*is_available=*/false),
      value_topic_(value_topic),
      availability_topic_(availability_topic) {}

void MQTTVerticalBarHorizontalListItem::HandleMessage(
    const deskmate::mqtt::MQTTMessage& msg) {
  if (msg.topic == value_topic_) {
    percentage_ = std::stod(msg.payload);
  } else if (msg.topic == availability_topic_) {
    is_available_ = msg.payload == "online";
  }
}

const std::vector<std::string>
MQTTVerticalBarHorizontalListItem::SubscriptionTopics() const {
  return {value_topic_, availability_topic_};
}

}  // namespace components
}  // namespace gfx
}  // namespace deskmate