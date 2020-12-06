#ifndef DESKMATE_GFX_COMPONENTS_VERTICAL_BARRIZONTAL_LIST_ITEM_H
#define DESKMATE_GFX_COMPONENTS_VERTICAL_BARRIZONTAL_LIST_ITEM_H

#include "deskmate/gfx/display.h"
#include "deskmate/gfx/screens/horizontal_list.h"

namespace deskmate {
namespace gfx {
namespace components {

class VerticalBarHorizontalListItem
    : public deskmate::gfx::screens::HorizontalListItem {
 public:
  VerticalBarHorizontalListItem(const std::string& display_name,
                                double percentage, bool is_available);

  void RenderBody(deskmate::gfx::Display* display,
                  bool is_selected) const override;

  void RenderLegend(Display* display) const override;

 protected:
  const std::string display_name_;
  double percentage_;
  bool is_available_;
};

}  // namespace components
}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_COMPONENTS_VERTICAL_BARRIZONTAL_LIST_ITEM_H