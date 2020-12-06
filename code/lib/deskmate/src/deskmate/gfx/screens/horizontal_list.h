#ifndef DESKMATE_GFX_SCREENS_HORIZONTAL_LIST_H
#define DESKMATE_GFX_SCREENS_HORIZONTAL_LIST_H

#include <memory>
#include <vector>

#include "deskmate/gfx/display.h"
#include "deskmate/gfx/screen.h"
#include "deskmate/input/input.h"

namespace deskmate {
namespace gfx {
namespace screens {

class HorizontalListItem {
 public:
  virtual ~HorizontalListItem() = default;
  virtual void RenderBody(deskmate::gfx::Display* display,
                          bool is_selected) const = 0;
  virtual void RenderLegend(deskmate::gfx::Display* display) const = 0;
  // Defaults to no-op.
  virtual void OnPress(){};
};

// A horizontal list that renders scrollable HorizontalListItems.
class HorizontalList : public Screen {
 public:
  HorizontalList(std::vector<std::unique_ptr<HorizontalListItem>>& items,
                 int item_width)
      : items_(std::move(items)),
        selected_(0),
        top_index_(0),
        item_width_(item_width) {}
  ~HorizontalList() override;
  void HandleInputEvent(deskmate::input::InputEvent event) override;
  void Render(Display* display) const override;

 private:
  std::vector<std::unique_ptr<HorizontalListItem>> items_;
  std::size_t selected_;

  // This is a value that controls the scrolling position. It might be modified
  // from const references, since it's just an internal state used for
  // rendering.
  mutable std::size_t top_index_;

  int item_width_;

  mutable deskmate::input::InputEvent last_scroll_ =
      deskmate::input::InputEvent::kUnknown;
};

}  // namespace screens
}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_SCREENS_HORIZONTAL_LIST_H