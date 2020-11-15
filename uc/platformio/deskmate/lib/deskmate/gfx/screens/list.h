#ifndef DESKMATE_GFX_SCREENS_LIST_H
#define DESKMATE_GFX_SCREENS_LIST_H

#include <memory>
#include <vector>

#include "deskmate/gfx/screen.h"
#include "deskmate/input/input.h"

namespace deskmate {
namespace gfx {
namespace screens {

class ListItem {
 public:
  virtual std::string Render() const = 0;
  virtual void OnSelect() = 0;
};

class ListScreen : public Screen {
 public:
  // ListScreen is constructed with a vector of non-owned pointers
  // to ListItems.
  explicit ListScreen(std::vector<ListItem*>& items)
      : items_(std::move(items)), selected_(0), top_index_(0) {}
  ~ListScreen() override;
  void HandleInputEvent(deskmate::input::InputEvent event) override;
  void Render(Display* display) const override;

 private:
  std::vector<ListItem*> items_;
  std::size_t selected_;
  std::size_t top_index_;
};

}  // namespace screens
}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_SCREENS_LIST_H