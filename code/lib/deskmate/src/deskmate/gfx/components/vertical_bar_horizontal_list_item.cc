#include "deskmate/gfx/components/vertical_bar_horizontal_list_item.h"

#include <cmath>

#include "deskmate/gfx/constants.h"
#include "deskmate/gfx/display.h"
#include "deskmate/gfx/screens/horizontal_list.h"
#include "deskmate/utils/utils.h"

namespace deskmate {
namespace gfx {
namespace components {
namespace {
using deskmate::gfx::Color;
using deskmate::gfx::Display;
using deskmate::gfx::Point;
using deskmate::gfx::Rect;
using deskmate::gfx::Size;
using deskmate::gfx::constants::kPadding;

// Radius for the little circle that is drawn under the currently selected item.
constexpr int kSelectorHeight = 16;
constexpr int kSelectorRadius = 6;
constexpr int kBaselineHeight = 1;
constexpr int kLegendFontScale = 2;
}  // namespace

VerticalBarHorizontalListItem::VerticalBarHorizontalListItem(
    const std::string& display_name, double percentage, bool is_available)
    : display_name_(display_name),
      percentage_(percentage),
      is_available_(is_available) {}

void VerticalBarHorizontalListItem::RenderBody(deskmate::gfx::Display* display,
                                               bool is_selected) const {
  const Size container = display->GetSize();

  Rect selector_rect{Point{container.height - kSelectorHeight, 0},
                     Size{kSelectorHeight, container.width}};

  // Maybe draw a little circle under the bar if this item is currently
  // selected.
  if (is_selected) {
    display->PushWindow(selector_rect);
    display->FillCircle(
        Point{selector_rect.size.height / 2, selector_rect.size.width / 2},
        kSelectorRadius, Color::kBlack);
    display->PopWindow();
  }

  // Bar container take the remainder of the space.
  Rect bar_container{
      Point{0, 0},
      Size{container.height - selector_rect.size.height, container.width}};

  // Bar itself.
  const int bar_height = percentage_ * bar_container.size.height;
  const int bar_width = bar_container.size.width / 2;
  Rect bar{Point{bar_container.size.height - bar_height,
                 (bar_container.size.width - bar_width) / 2},
           Size{bar_height, bar_width}};
  if (is_available_) {
    display->FillRect(bar, Color::kBlack);
  } else {
    display->DrawRect(bar, Color::kBlack);
  }

  // Bar baseline.
  Rect base{Point{bar_container.size.height - kBaselineHeight, 0},
            Size{kBaselineHeight, container.width}};
  display->FillRect(base, Color::kBlack);
}

void VerticalBarHorizontalListItem::RenderLegend(Display* display) const {
  const int char_scale = kLegendFontScale;
  const Size container = display->GetSize();
  const Size char_size = display->GetCharSize();

  // Value goes on the right.
  std::string legend = deskmate::utils::to_fixed(100 * percentage_, 1) + "%";
  const int legend_width = char_scale * char_size.width * legend.length();
  display->PutText((container.height - char_size.height * char_scale) / 2,
                   container.width - legend_width, legend, char_scale,
                   Color::kBlack);

  // Display name goes on the left.
  const int chars_left =
      (container.width - legend_width) / (char_scale * char_size.width);
  std::string display_name(display_name_);
  if (static_cast<int>(display_name_.length()) > chars_left) {
    display_name = display_name.substr(0, chars_left - 1) + ".";
  }
  display->PutText((container.height - char_size.height * char_scale) / 2, 0,
                   display_name, char_scale, Color::kBlack);
}

}  // namespace components
}  // namespace gfx
}  // namespace deskmate