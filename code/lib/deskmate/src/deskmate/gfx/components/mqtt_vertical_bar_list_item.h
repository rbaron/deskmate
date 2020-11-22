#ifndef DESKMATE_GFX_COMPONENTS_MQTT_VERTICAL_BAR_LIST_ITEM_H
#define DESKMATE_GFX_COMPONENTS_MQTT_VERTICAL_BAR_LIST_ITEM_H

#include <string>

#include "deskmate/gfx/display.h"
#include "deskmate/gfx/screens/vertical_bars_list.h"
#include "deskmate/mqtt/mqtt.h"

namespace deskmate {
namespace gfx {
namespace components {

// Represents a VerticalBarListItem that listens to a MQTT topic containing a
// floating point.
class MQTTVerticalBarListItem
    : public deskmate::gfx::screens::VerticalBarListItem,
      public deskmate::mqtt::MQTTSubscriber {
 public:
  MQTTVerticalBarListItem(const std::string& display_name,
                          const std::string& value_topic,
                          const std::string& availability_topic);

  double Percentage() const override;
  bool IsFilled() const override;
  const std::string& DisplayName() const override;

  void HandleMessage(const deskmate::mqtt::MQTTMessage& msg) override;
  const std::vector<std::string> SubscriptionTopics() const override;

 private:

  std::string display_name_;
  std::string value_topic_;
  std::string availability_topic_;
  bool is_available_ = false;
  double percentage_ = 0;
};

}  // namespace components
}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_COMPONENTS_MQTT_VERTICAL_BAR_LIST_ITEM_H