#ifndef DESKMATE_GFX_SCREENS_LIST_H
#define DESKMATE_GFX_SCREENS_LIST_H

#include <memory>
#include <vector>

#include "deskmate/input/input.h"
#include "deskmate/gfx/screen.h"

namespace deskmate {
namespace gfx {
namespace screens {

struct ListItem {
  std::string name;
};

class ListScreen : public Screen {
 public:
  explicit ListScreen(std::vector<ListItem>& items):
      items_(std::move(items)),
      selected_(0),
      top_index_(0) {}
  ~ListScreen() override;
  void HandleInputEvent(deskmate::input::InputEvent event) override;
  void Render(Display *display) const override;

 private:
  std::vector<ListItem> items_;
  std::size_t selected_;
  std::size_t top_index_;
};

}  // namespace deskmate
}  // namespace gfx
}  // namespace screens

#endif  // DESKMATE_GFX_SCREENS_LIST_H