#ifndef DESKMATE_GFX_COMPONENTS_TEXT_LIST_ITEM_H
#define DESKMATE_GFX_COMPONENTS_TEXT_LIST_ITEM_H

#include "deskmate/gfx/screens/list.h"

namespace deskmate {
namespace gfx {
namespace components {

namespace {
using deskmate::gfx::screens::ListItem;
}  // namespace

// Represents a ListItem that issues MQTT messages when clicked and listens and
// reacts to a topic.
class TextListItem : public ListItem {
 public:
  explicit TextListItem(const std::string& display_name)
      : display_name_(display_name) {}

  std::string Render() const override { return display_name_; }

  // Do nothing.
  void OnSelect() override {}

 private:
  std::string display_name_;
};

}  // namespace components
}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_COMPONENTS_TEXT_LIST_ITEM_H