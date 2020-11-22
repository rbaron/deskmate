#include "deskmate/gfx/screens/list.h"

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

constexpr unsigned int kItemHeight = 32;
// constexpr unsigned int kFontScale = 2;
}  // namespace

ListScreen::~ListScreen() {}

void ListScreen::HandleInputEvent(InputEvent event) {
  switch (event) {
    case InputEvent::kCrankCW:
      selected_ = selected_ < items_.size() - 1 ? selected_ + 1 : selected_;
      // If the end of the selected element is bigger, scroll
      // if ((selected_ + 1) * kItemHeight)
      last_scroll_ = event;
      break;
    case InputEvent::kCrankCCW:
      selected_ = selected_ > 0 ? selected_ - 1 : 0;
      last_scroll_ = event;
      break;
    case InputEvent::kAPush:
      items_[selected_]->OnSelect();
      break;
    default:
      break;
  }
}

void ListScreen::Render(Display* display) const {
  const Size& window_size = display->GetSize();

  // Maybe scroll.
  if (last_scroll_ == InputEvent::kCrankCW) {
    if ((selected_ - top_index_ + 1) * kItemHeight > window_size.height) {
      top_index_++;
    }
  } else if (last_scroll_ == InputEvent::kCrankCCW) {
    if (selected_ < top_index_) {
      top_index_ = selected_;
    }
  }
  last_scroll_ = InputEvent::kUnknown;

  for (size_t index = top_index_;
       index < items_.size() &&
       (index - top_index_) * kItemHeight < display->GetSize().height;
       index++) {
    Rect item_window{
        Point{static_cast<unsigned int>((index - top_index_) * kItemHeight), 0},
        {kItemHeight, window_size.width}};
    display->PushWindow(item_window);
    items_[index]->Render(display, index == selected_);
    display->PopWindow();
  }
}

}  // namespace screens
}  // namespace gfx
}  // namespace deskmate