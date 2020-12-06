#ifndef DESKMATE_GFX_COMPONENTS_TEXT_LIST_ITEM_H
#define DESKMATE_GFX_COMPONENTS_TEXT_LIST_ITEM_H

#include "deskmate/gfx/constants.h"
#include "deskmate/gfx/screens/list.h"

namespace deskmate {
namespace gfx {
namespace components {

namespace {
using deskmate::gfx::constants::kPadding;
using deskmate::gfx::screens::ListItem;
}  // namespace

// Represents a ListItem that issues MQTT messages when clicked and listens and
// reacts to a topic.
class TextListItem : public ListItem {
 public:
  explicit TextListItem(const std::string& display_name)
      : display_name_(display_name) {}

  void Render(Display* display, bool is_selected) const override {
    const int char_scale = 2;
    const Size& size = display->GetSize();
    const Size& char_size = display->GetCharSize();
    std::string text = display_name_;
    if (static_cast<int>(text.length()) * char_scale * char_size.width >
        size.width) {
      text = text.substr(0, size.width / (char_scale * char_size.width) - 1);
      text += ".";
    }
    display->PutText(kPadding,
                     (size.height - char_scale * char_size.height) / 2, text,
                     char_scale, is_selected ? Color::kWhite : Color::kBlack,
                     is_selected ? Color::kBlack : Color::kWhite);
  }

  // Do nothing.
  void OnSelect() override {}

 private:
  std::string display_name_;
};

}  // namespace components
}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_COMPONENTS_TEXT_LIST_ITEM_H