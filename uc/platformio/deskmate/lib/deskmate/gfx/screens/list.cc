#include "deskmate/gfx/screens/list.h"

#include <Arduino.h>

#include <algorithm>

#include "deskmate/gfx/display.h"
#include "deskmate/input/input.h"

namespace deskmate {
namespace gfx {
namespace screens {

namespace {
using deskmate::gfx::Color;
using deskmate::gfx::Rect;
using deskmate::input::InputEvent;
}  // namespace

ListScreen::~ListScreen() {}

void ListScreen::HandleInputEvent(InputEvent event) {
  switch (event) {
    case InputEvent::kCrankCW:
      selected_ = selected_ < items_.size() - 1 ? selected_ + 1 : selected_;
      break;
    case InputEvent::kCrankCCW:
      selected_ = selected_ > 0 ? selected_ - 1 : 0;
      break;
    case InputEvent::kAPush:
      items_[selected_]->OnSelect();
      break;
    default:
      break;
  }
}

void ListScreen::Render(Display *display) const {
  uint8_t font_scale = 2;
  uint8_t line_height = font_scale * display->GetCharSize().height;
  const Size& window_size = display->GetSize();
  for (size_t index = top_index_;
       index < items_.size() && index * line_height < display->GetSize().height;
       index++) {
    Rect item_window{Point{(index - top_index_) * line_height, 0}, {line_height, window_size.width}};
    display->PushWindow(item_window);
    items_[index]->Render(display, index == selected_);
    display->PopWindow();
  }
}

}  // namespace screens
}  // namespace gfx
}  // namespace deskmate