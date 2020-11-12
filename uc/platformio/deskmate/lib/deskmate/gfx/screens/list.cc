#include "deskmate/gfx/screens/list.h"

#include <Arduino.h>
#include <algorithm>

#include "deskmate/input/input.h"

namespace deskmate {
namespace gfx {
namespace screens {

namespace {

using deskmate::input::InputEvent;

}  // namespace

ListScreen::~ListScreen() {
}

void ListScreen::HandleInputEvent(InputEvent event) {
  switch (event) {
    case InputEvent::kCrankCW:
      selected_ = selected_ < items_.size() - 1 ? selected_ + 1 : selected_;
      dirty_ = true;
      break;
    case InputEvent::kCrankCCW:
      selected_ = selected_ > 0 ? selected_ - 1 : 0;
      dirty_ = true;
      break;
    case InputEvent::kAPush:
      items_[selected_]->OnSelect();
      break;
    default:
      break;
  }
}

void ListScreen::Render(Display *display) const {
  if (!dirty_) {
    return;
  }
  display->Clear();
  uint8_t font_size = 2;
  uint8_t line_height = font_size * 8;
  for (
    size_t index = top_index_;
    index < items_.size() && index * line_height < display->Height();
    index++
  ) {
    if (index == selected_) {
      display->PutText((index - top_index_) * line_height, 0, items_[index]->Render(), font_size, true, false);
    } else {
      display->PutText((index - top_index_) * line_height, 0, items_[index]->Render(), font_size, false);
    }
  }
  display->Refresh();
  dirty_ = false;
}

}  // namespace deskmate
}  // namespace gfx
}  // namespace screens