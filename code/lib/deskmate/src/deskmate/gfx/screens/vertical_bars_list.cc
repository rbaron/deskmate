#include "deskmate/gfx/screens/vertical_bars_list.h"

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

// Radius for the little circle that is drawn under the currently selected item.
constexpr unsigned int kSelectorRadius = 6;
constexpr unsigned int kBarWidth = 2 * kPadding;
constexpr unsigned int kItemWidth = kBarWidth + 2 * kPadding;
constexpr unsigned int kLegendFontScale = 2;

// Scrollbar.
constexpr unsigned int kScrollbarHeight = 3;

void DrawItem(Display* display, const VerticalBarListItem& item) {
  const Size container = display->GetSize();
  // Center the bar horizontally inside container.
  Rect bar_rect{Point{static_cast<unsigned int>((1 - item.Percentage()) *
                                                container.height),
                      (container.width - kBarWidth) / 2},
                Size{static_cast<unsigned int>(
                         // std::ceil so we fill exactly until the bottom.
                         std::ceil(item.Percentage() * container.height)),
                     kBarWidth}};

  if (item.IsFilled()) {
    display->FillRect(bar_rect, Color::kBlack);
  } else {
    display->DrawRect(bar_rect, Color::kBlack);
  }
}
}  // namespace

VerticalBarsList::~VerticalBarsList() = default;

void VerticalBarsList::HandleInputEvent(InputEvent event) {
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

void VerticalBarsList::Render(Display* display) const {
  const Size& window_size = display->GetSize();

  // Push inner padded window.
  display->PushWindow({{kPadding, kPadding},
                       {window_size.height, window_size.width - 2 * kPadding}});

  const Size& inner_size = display->GetSize();

  const unsigned int text_height =
      2 * kPadding + kLegendFontScale * display->GetCharSize().height;

  const unsigned int selector_height = 2 * kPadding + 2 * kSelectorRadius;
  const unsigned int divider_height = 1;
  const unsigned int top_bars_y = 0;

  const unsigned int bottom_bars_y =
      inner_size.height - text_height - selector_height - divider_height;

  // const unsigned int offset_x = kPadding;

  const int n_items_to_show = inner_size.width / kItemWidth;
  const bool draw_scrollbar =
      items_.size() > 1 && items_.size() > n_items_to_show;

  // Maybe scroll.
  if (last_scroll_ == InputEvent::kCrankCW) {
    if ((selected_ - top_index_ + 1) * kItemWidth > inner_size.width) {
      top_index_++;
    }
  } else if (last_scroll_ == InputEvent::kCrankCCW) {
    if (selected_ < top_index_) {
      top_index_ = selected_;
    }
  }
  last_scroll_ = InputEvent::kUnknown;

  // Draw items.
  for (int i = 0; i < n_items_to_show && i + top_index_ < items_.size(); i++) {
    const auto& item = items_[i + top_index_];
    Rect item_rect = {Point{0, i * kItemWidth},
                      Size{bottom_bars_y - top_bars_y, kItemWidth}};
    display->PushWindow(item_rect);
    DrawItem(display, *item);
    display->PopWindow();

    // Maybe draw the selected mark.
    if (i + top_index_ == selected_) {
      Point selector_center{
          bottom_bars_y + divider_height + kPadding + kSelectorRadius,
          item_rect.point.x + item_rect.size.width / 2,
      };
      display->FillCircle(selector_center, kSelectorRadius, Color::kBlack);
    }
  }

  Rect divider_rect{{bottom_bars_y - 1, 0}, {divider_height, inner_size.width}};
  display->DrawRect(divider_rect, Color::kBlack);

  if (draw_scrollbar) {
    const unsigned int scrollbar_width =
        static_cast<double>(n_items_to_show) / items_.size() * inner_size.width;
    const unsigned int scrollbar_left_pixel =
        static_cast<double>(inner_size.width - scrollbar_width) /
        (items_.size() - n_items_to_show) * top_index_;
    Rect scroll_rect{Point{divider_rect.point.y + divider_rect.size.height,
                           scrollbar_left_pixel},
                     {kScrollbarHeight, scrollbar_width}};
    display->FillRect(scroll_rect, Color::kBlack);
  }

  // Name.
  // TODO: ellipsis if name is too long.
  display->PutText(
      divider_rect.point.y + divider_rect.size.height + selector_height, 0,
      items_[selected_]->DisplayName(), kLegendFontScale, Color::kBlack);

  // Percentage.
  int percentage = 100 * items_[selected_]->Percentage();
  const std::string percentage_str = std::to_string(percentage) + "%";
  display->PutText(
      divider_rect.point.y + divider_rect.size.height + selector_height,
      inner_size.width - percentage_str.length() * kLegendFontScale *
                             display->GetCharSize().width,
      percentage_str, kLegendFontScale, Color::kBlack);

  // Pop the padded window.
  display->PopWindow();
}

}  // namespace screens
}  // namespace gfx
}  // namespace deskmate