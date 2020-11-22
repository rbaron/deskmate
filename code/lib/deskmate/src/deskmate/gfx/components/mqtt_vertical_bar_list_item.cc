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
    const std::string& display_name, const std::string& value_topic)
    : display_name_(display_name), value_topic_(value_topic) {}

std::string MQTTVerticalBarListItem::GetSubscriptionTopic() const {
  return value_topic_;
};

double MQTTVerticalBarListItem::Percentage() const { return percentage_; }
bool MQTTVerticalBarListItem::IsFilled() const { return is_available_; }
const std::string& MQTTVerticalBarListItem::DisplayName() const {
  return display_name_;
}

void MQTTVerticalBarListItem::HandleOwnMessage(
    const deskmate::mqtt::MQTTMessage& msg) {
  std::cout << "Got mqtt msg: " << msg.topic << " -> " << msg.payload
            << std::endl;
  // TODO: Handle error. Catch invalid_argument and out_of_range exceptions.
  // http://www.cplusplus.com/reference/string/stod/
  percentage_ = std::stod(msg.payload);
};

}  // namespace components
}  // namespace gfx
}  // namespace deskmate