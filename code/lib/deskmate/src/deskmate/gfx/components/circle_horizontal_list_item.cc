#include "deskmate/gfx/components/circle_horizontal_list_item.h"

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
using deskmate::gfx::Size;
using deskmate::gfx::constants::kPadding;
}  // namespace

CircleHorizontalListItem::CircleHorizontalListItem(std::string display_name,
                                                   std::string unit,
                                                   double value,
                                                   bool is_available)
    : display_name_(std::move(display_name)),
      unit_(std::move(unit)),
      value_(value),
      is_available_(is_available) {}

void CircleHorizontalListItem::RenderBody(deskmate::gfx::Display* display,
                                          bool is_selected) const {
  const int char_scale = 2;
  const Size container = display->GetSize();
  const Size char_size = display->GetCharSize();

  const Point center{container.height / 2, container.width / 2};
  const int outer_radius = (container.width - kPadding) / 2;
  const int inner_radius = outer_radius - (is_selected ? 4 : 1);

  // Draw black circle.
  display->FillCircle(center, outer_radius, Color::kBlack);

  // Draw inner white circle.
  display->FillCircle(center, inner_radius, Color::kWhite);

  // Draw text in the middle.
  const std::string text =
      is_available_
          ? std::to_string(static_cast<int>(std::round(value_))) + unit_
          : "N/A";

  const Point text_point = {center.y - char_scale * char_size.height / 2,
                            center.x - char_scale * char_size.width *
                                           static_cast<int>(text.length()) / 2};
  display->PutText(text_point.y, text_point.x, text, char_scale, Color::kBlack);
}

void CircleHorizontalListItem::RenderLegend(Display* display) const {
  const int char_scale = 2;
  const Size container = display->GetSize();
  const Size char_size = display->GetCharSize();

  // Value goes on the right.
  std::string legend = deskmate::utils::to_fixed(value_, 1) + unit_;
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