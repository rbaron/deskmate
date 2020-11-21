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
                          const std::string& subscription_topic,
                          deskmate::mqtt::MQTTMessageQueue* mqtt_out_queue);

  std::string GetSubscriptionTopic() const override;

  double Percentage() const override;
  bool IsFilled() const override;
  const std::string& DisplayName() const override;

 private:
  void HandleOwnMessage(const deskmate::mqtt::MQTTMessage& msg) override;

  std::string display_name_;
  std::string value_topic_;
  deskmate::mqtt::MQTTMessageQueue* mqtt_out_queue_;
  bool is_available_ = false;
  double percentage_ = 0;
};

}  // namespace components
}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_COMPONENTS_MQTT_VERTICAL_BAR_LIST_ITEM_H