#include "deskmate/gfx/screens/list.h"

#include <Arduino.h>
#include <algorithm>

#include "deskmate/input/input.h"

namespace deskmate {
namespace gfx {
namespace screens {

namespace {

using deskmate::input::InputEvent;

}  // namespace

ListScreen::~ListScreen() {
}

void ListScreen::HandleInputEvent(InputEvent event) {
  if (event == InputEvent::kCrankCW) {
    selected_ = selected_ < items_.size() - 1 ? selected_ + 1 : selected_;
    // Serial.printf("New selected: %d\n", selected_);
  } else if (event == InputEvent::kCrankCCW) {
    selected_ = selected_ > 0 ? selected_ - 1 : 0;
    // Serial.printf("New selected: %d\n", selected_);
  } else if (event == InputEvent::kAPush) {
    Serial.printf("A Pushed\n");
  } else if (event == InputEvent::kBPush) {
    Serial.printf("B Pushed\n");
  } else if (event == InputEvent::kCPush) {
    Serial.printf("C Pushed\n");
  }
}

void ListScreen::Render(Display *display) const {
  display->Clear();
  uint8_t font_size = 2;
  uint8_t line_height = font_size * 8;
  for (
    size_t index = top_index_;
    index < items_.size() && index * line_height < display->Height();
    index++
   ) {
     if (index == selected_) {
      display->PutText((index - top_index_) * line_height, 0, items_[index].name, font_size, true, false);
     } else {
      display->PutText((index - top_index_) * line_height, 0, items_[index].name, font_size, false);
     }
   }
   display->Refresh();
}

}  // namespace deskmate
}  // namespace gfx
}  // namespace screens