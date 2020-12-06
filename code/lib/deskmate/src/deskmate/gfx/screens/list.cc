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

constexpr int kItemHeight = 32;

// Scrollbar
constexpr int kScrollBarWidth = 4;
}  // namespace

ListScreen::~ListScreen() {}

void ListScreen::HandleInputEvent(InputEvent event) {
  switch (event) {
    case InputEvent::kCrankCW:
      selected_ = selected_ < items_.size() - 1 ? selected_ + 1 : selected_;
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
  if (items_.empty()) {
    display->PutText(0, 0, "<Empty>", /*scale=*/2, Color::kBlack);
    return;
  }

  const Size& window_size = display->GetSize();

  // Maybe scroll.
  if (last_scroll_ == InputEvent::kCrankCW) {
    if (static_cast<int>(selected_ - top_index_ + 1) * kItemHeight >
        window_size.height) {
      top_index_++;
    }
  } else if (last_scroll_ == InputEvent::kCrankCCW) {
    if (selected_ < top_index_) {
      top_index_ = selected_;
    }
  }
  last_scroll_ = InputEvent::kUnknown;

  // We only show whole items for now.
  const std::size_t n_items_to_show = window_size.height / kItemHeight;

  const bool draw_scrollbar =
      items_.size() > 1 && items_.size() > n_items_to_show;

  for (std::size_t i = 0; i < n_items_to_show && i + top_index_ < items_.size();
       i++) {
    const std::size_t item_index = i + top_index_;
    Rect item_window{Point{static_cast<int>(i * kItemHeight), 0},
                     {kItemHeight, window_size.width -
                                       (draw_scrollbar ? kScrollBarWidth : 0)}};
    display->PushWindow(item_window);
    items_[i + top_index_]->Render(display, item_index == selected_);
    display->PopWindow();
  }

  // Draw scrollbar.
  if (draw_scrollbar) {
    const int scrollbar_height = static_cast<double>(n_items_to_show) /
                                 items_.size() * window_size.height;
    const int scrollbar_top_pixel =
        static_cast<double>(window_size.height - scrollbar_height) /
        (items_.size() - n_items_to_show) * top_index_;
    Rect scroll_rect{
        Point{scrollbar_top_pixel, window_size.width - kScrollBarWidth},
        {scrollbar_height, kScrollBarWidth}};
    display->FillRect(scroll_rect, Color::kBlack);
  }
}

}  // namespace screens
}  // namespace gfx
}  // namespace deskmate