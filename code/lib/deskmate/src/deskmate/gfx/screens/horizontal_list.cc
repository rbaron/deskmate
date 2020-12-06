#include "deskmate/gfx/screens/horizontal_list.h"

#include <algorithm>
#include <cmath>

#include "deskmate/gfx/constants.h"
#include "deskmate/gfx/display.h"
#include "deskmate/input/input.h"

namespace deskmate {
namespace gfx {
namespace screens {

namespace {
using deskmate::gfx::Color;
using deskmate::gfx::Rect;
using deskmate::gfx::Size;
using deskmate::gfx::constants::kPadding;
using deskmate::input::InputEvent;

constexpr int kLegendCharScale = 2;

// Scrollbar.
constexpr int kScrollbarVerticalPadding = kPadding;
constexpr int kScrollbarHeight = 3;
}  // namespace

HorizontalList::~HorizontalList() = default;

void HorizontalList::HandleInputEvent(InputEvent event) {
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
      items_[selected_]->OnPress();
      break;
    default:
      break;
  }
}

void HorizontalList::Render(Display* display) const {
  const Size char_size = display->GetCharSize();
  const Size window_size = display->GetSize();

  // Push inner padded window.
  display->PushWindow(
      {{kPadding, kPadding},
       {window_size.height - 2 * kPadding, window_size.width - 2 * kPadding}});
  const Size inner_size = display->GetSize();

  // Legend area. Goes in the bottom.
  const int legend_height = kLegendCharScale * char_size.height;
  const Rect legend_rect{Point{inner_size.height - legend_height, 0},
                         Size{legend_height, inner_size.width}};

  // Scrollbar area. Goes on top of the legend.
  const Rect scrollbar_rect{
      legend_rect.point -
          Point{kScrollbarHeight + 2 * kScrollbarVerticalPadding, 0},
      Size{kScrollbarHeight + 2 * kScrollbarVerticalPadding, inner_size.width}};

  // Items area. It takes the remaining space, on top of the scrollbar.
  const Rect items_rect{Point{0, 0},
                        Size{scrollbar_rect.point.y, inner_size.width}};

  const std::size_t n_items_to_show = inner_size.width / item_width_;
  const bool should_draw_scrollbar =
      items_.size() > 1 && items_.size() > n_items_to_show;

  // Maybe scroll.
  if (last_scroll_ == InputEvent::kCrankCW) {
    if (static_cast<int>(selected_ - top_index_ + 1) * item_width_ >
        inner_size.width) {
      top_index_++;
    }
  } else if (last_scroll_ == InputEvent::kCrankCCW) {
    if (selected_ < top_index_) {
      top_index_ = selected_;
    }
  }
  last_scroll_ = InputEvent::kUnknown;

  // Draw items.
  display->PushWindow(items_rect);
  for (std::size_t i = 0; i < n_items_to_show && i + top_index_ < items_.size();
       i++) {
    const auto& item = items_[i + top_index_];
    Rect item_rect = {Point{0, static_cast<int>(i) * item_width_},
                      Size{items_rect.size.height, item_width_}};

    bool is_selected = i + top_index_ == selected_;

    display->PushWindow(item_rect);
    item->RenderBody(display, is_selected);
    display->PopWindow();

    // Maybe draw selected item's legend.
    if (is_selected) {
      display->PushWindow(legend_rect);
      item->RenderLegend(display);
      display->PopWindow();
    }
  }
  // Pop items window.
  display->PopWindow();

  if (should_draw_scrollbar) {
    display->PushWindow(scrollbar_rect);
    const int scrollbar_width =
        static_cast<double>(n_items_to_show) / items_.size() * inner_size.width;
    const int scrollbar_left_pixel =
        static_cast<double>(inner_size.width - scrollbar_width) /
        (items_.size() - n_items_to_show) * top_index_;
    Rect scrollbar_bar_rect{
        Point{kScrollbarVerticalPadding, scrollbar_left_pixel},
        Size{kScrollbarHeight, scrollbar_width}};
    display->FillRect(scrollbar_bar_rect, Color::kBlack);
    display->PopWindow();
  }

  // Pop the inner padded window.
  display->PopWindow();
}

}  // namespace screens
}  // namespace gfx
}  // namespace deskmate