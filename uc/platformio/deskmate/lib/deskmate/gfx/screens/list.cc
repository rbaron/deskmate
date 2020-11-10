
#include "deskmate/gfx/screens/list.h"

namespace deskmate {
namespace gfx {
namespace screens {

ListScreen::~ListScreen() {
}

void ListScreen::HandleInputs(const deskmate::input::InputState& input_state) {
}

void ListScreen::Render(Display *display) const {
  display->Clear();
  uint8_t font_size = 2;
  uint8_t line_height = font_size * 8;
  for (
    unsigned int index = top_index_;
    index < items_.size() && index * line_height < display->Height();
    index++
   ) {
     display->PutText((index - top_index_) * line_height, 0, items_[index].name, font_size, false);
    //  display->PutText((index - top_index_) * line_height, 0, e, font_size, false);
   }
   display->Refresh();
}

}  // namespace deskmate
}  // namespace gfx
}  // namespace screens