#ifndef DESKMATE_GFX_SCREENS_LIST_H
#define DESKMATE_GFX_SCREENS_LIST_H

#include <memory>
#include <vector>

#include "deskmate/gfx/display.h"
#include "deskmate/gfx/screen.h"
#include "deskmate/input/input.h"

namespace deskmate {
namespace gfx {
namespace screens {

class ListItem {
 public:
  virtual ~ListItem() = default;
  virtual void Render(Display* display, bool is_selected) const = 0;
  virtual void OnSelect() = 0;
};

class ListScreen : public Screen {
 public:
  explicit ListScreen(std::vector<std::unique_ptr<ListItem>>& items)
      : items_(std::move(items)), selected_(0), top_index_(0) {}
  ~ListScreen() override;
  void HandleInputEvent(deskmate::input::InputEvent event) override;
  void Render(Display* display) const override;

 private:
  std::vector<std::unique_ptr<ListItem>> items_;
  std::size_t selected_;

  // These are internal states which can be updated from a const reference.
  mutable std::size_t top_index_;
  mutable deskmate::input::InputEvent last_scroll_ =
      deskmate::input::InputEvent::kUnknown;
};

}  // namespace screens
}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_SCREENS_LIST_H