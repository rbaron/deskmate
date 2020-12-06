#ifndef DESKMATE_GFX_COMPONENTS_CIRCLE_HORIZONTAL_LIST_ITEM_H
#define DESKMATE_GFX_COMPONENTS_CIRCLE_HORIZONTAL_LIST_ITEM_H

#include "deskmate/gfx/display.h"
#include "deskmate/gfx/screens/horizontal_list.h"

namespace deskmate {
namespace gfx {
namespace components {

class CircleHorizontalListItem
    : public deskmate::gfx::screens::HorizontalListItem {
 public:
  CircleHorizontalListItem(std::string display_name, std::string unit,
                           double value, bool is_available);

  void RenderBody(deskmate::gfx::Display* display,
                  bool is_selected) const override;

  void RenderLegend(Display* display) const override;

 protected:
  const std::string display_name_;
  const std::string unit_;
  double value_;
  bool is_available_;
};

}  // namespace components
}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_COMPONENTS_CIRCLE_HORIZONTAL_LIST_ITEM_H