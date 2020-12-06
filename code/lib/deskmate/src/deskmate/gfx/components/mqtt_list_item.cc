#include "deskmate/gfx/components/mqtt_list_item.h"

#include "deskmate/gfx/display.h"
#include "deskmate/gfx/screens/list.h"
#include "deskmate/mqtt/mqtt.h"

namespace deskmate {
namespace gfx {
namespace components {
namespace {
using deskmate::gfx::Color;
using deskmate::gfx::Display;
using deskmate::gfx::Point;
using deskmate::gfx::Rect;
using deskmate::gfx::Size;
using deskmate::gfx::screens::ListItem;
using deskmate::mqtt::MQTTMessage;
using deskmate::mqtt::MQTTMessageBuffer;
using deskmate::mqtt::MQTTSubscriber;

constexpr int kCircleRadius = 8;
constexpr int kPadding = 10;
}  // namespace

MQTTListItem::MQTTListItem(const std::string& display_name,
                           const std::string& command_topic,
                           const std::string& state_topic,
                           MQTTMessageBuffer* mqtt_buffer)
    : display_name_(display_name),
      command_topic_(command_topic),
      state_topic_(state_topic),
      mqtt_buffer_(mqtt_buffer) {}

void MQTTListItem::Render(Display* display, bool is_selected) const {
  const int char_scale = 2;
  const Size& size = display->GetSize();
  const Size& char_size = display->GetCharSize();
  Point circle_center{size.height / 2, kCircleRadius + kPadding};
  if (on_) {
    display->FillCircle(circle_center, kCircleRadius, Color::kBlack);
  } else {
    display->DrawCircle(circle_center, kCircleRadius, Color::kBlack);
  }

  Point text_top{(size.height - char_scale * char_size.height) / 2,
                 circle_center.x + kCircleRadius + kPadding};
  display->PutText(text_top.y, text_top.x, display_name_, 2, Color::kBlack);

  if (is_selected) {
    Rect rect{{0, 0}, size};
    display->DrawRect(rect, Color::kBlack);
  }
}

void MQTTListItem::OnSelect() {
  // mqtt_buffer_->EnqueueForSending({command_topic_, on_ ? "OFF" : "ON"});
  mqtt_buffer_->Publish({command_topic_, on_ ? "OFF" : "ON"});
}

const std::vector<std::string> MQTTListItem::SubscriptionTopics() const {
  return {state_topic_};
}

void MQTTListItem::HandleMessage(const MQTTMessage& msg) {
  on_ = msg.payload == "ON";
}

}  // namespace components
}  // namespace gfx
}  // namespace deskmate