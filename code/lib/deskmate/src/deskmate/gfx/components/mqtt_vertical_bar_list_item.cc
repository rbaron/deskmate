#include "deskmate/gfx/components/mqtt_vertical_bar_list_item.h"

#include <iostream>
#include <string>

#include "deskmate/gfx/display.h"
#include "deskmate/gfx/screens/vertical_bars_list.h"
#include "deskmate/mqtt/mqtt.h"

namespace deskmate {
namespace gfx {
namespace components {

// Represents a VerticalBarListItem that listens to a MQTT topic containing a
// floating point.
MQTTVerticalBarListItem::MQTTVerticalBarListItem(
    const std::string& display_name, const std::string& value_topic,
    const std::string& availability_topic)
    : display_name_(display_name),
      value_topic_(value_topic),
      availability_topic_(availability_topic) {}

const std::vector<std::string> MQTTVerticalBarListItem::SubscriptionTopics()
    const {
  return {value_topic_, availability_topic_};
}

double MQTTVerticalBarListItem::Percentage() const { return percentage_; }

bool MQTTVerticalBarListItem::IsFilled() const { return is_available_; }

const std::string& MQTTVerticalBarListItem::DisplayName() const {
  return display_name_;
}

void MQTTVerticalBarListItem::HandleMessage(
    const deskmate::mqtt::MQTTMessage& msg) {
  if (msg.topic == availability_topic_) {
    is_available_ = msg.payload == "online";
  } else if(msg.topic == value_topic_) {
    // TODO: Handle error. Catch invalid_argument and out_of_range exceptions.
    // http://www.cplusplus.com/reference/string/stod/
    percentage_ = std::stod(msg.payload);
  }
};

}  // namespace components
}  // namespace gfx
}  // namespace deskmate