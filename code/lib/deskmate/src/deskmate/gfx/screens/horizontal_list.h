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
  explicit HorizontalList(
      std::vector<std::unique_ptr<HorizontalListItem>>& items)
      : items_(std::move(items)), selected_(0), top_index_(0) {}
  ~HorizontalList() override;
  void HandleInputEvent(deskmate::input::InputEvent event) override;
  void Render(Display* display) const override;

 private:
  std::vector<std::unique_ptr<HorizontalListItem>> items_;
  std::size_t selected_;

  // These are internal states which can be updated from a const reference.
  mutable std::size_t top_index_;
  mutable deskmate::input::InputEvent last_scroll_ =
      deskmate::input::InputEvent::kUnknown;
};

}  // namespace screens
}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_SCREENS_HORIZONTAL_LIST_H