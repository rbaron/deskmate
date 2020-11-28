#include "deskmate/gfx/components/circle_horizontal_list_item.h"

#include <cmath>

#include "deskmate/gfx/constants.h"
#include "deskmate/gfx/display.h"
#include "deskmate/gfx/screens/horizontal_list.h"

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

CircleHorizontalListItem::CircleHorizontalListItem(
    const std::string& display_name, const std::string& unit, double value,
    bool is_available)
    : display_name_(display_name),
      unit_(unit),
      value_(value),
      is_available_(is_available) {}

void CircleHorizontalListItem::RenderBody(deskmate::gfx::Display* display,
                                          bool is_selected) const {
  const unsigned int char_scale = 2;
  const Size container = display->GetSize();
  const Size char_size = display->GetCharSize();

  const Point center{container.height / 2, container.width / 2};
  const unsigned int outer_radius = (container.width - kPadding) / 2;
  const unsigned int inner_radius = outer_radius - (is_selected ? 4 : 1);

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
  const unsigned int char_scale = 2;
  const Size container = display->GetSize();
  const Size char_size = display->GetCharSize();
  display->PutText((container.height - char_size.height * char_scale) / 2, 0,
                   display_name_, 2, Color::kBlack);
}

}  // namespace components
}  // namespace gfx
}  // namespace deskmate