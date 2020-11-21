#ifndef DESKMATE_GFX_SCREENS_VERTICAL_BARS_LIST_H
#define DESKMATE_GFX_SCREENS_VERTICAL_BARS_LIST_H

#include <memory>
#include <vector>

#include "deskmate/gfx/display.h"
#include "deskmate/gfx/screen.h"
#include "deskmate/input/input.h"

namespace deskmate {
namespace gfx {
namespace screens {

class VerticalBarListItem {
 public:
  virtual ~VerticalBarListItem() = default;
  virtual const std::string& DisplayName() const = 0;
  virtual double Percentage() const = 0;
  virtual bool IsFilled() const = 0;
  // Defaults to no-op.
  virtual void OnSelect(){};
};

class VerticalBarsList : public Screen {
 public:
  explicit VerticalBarsList(
      std::vector<std::unique_ptr<VerticalBarListItem>>& items)
      : items_(std::move(items)), selected_(0), top_index_(0) {}
  ~VerticalBarsList() override;
  void HandleInputEvent(deskmate::input::InputEvent event) override;
  void Render(Display* display) const override;

 private:
  std::vector<std::unique_ptr<VerticalBarListItem>> items_;
  std::size_t selected_;
  std::size_t top_index_;
};

}  // namespace screens
}  // namespace gfx
}  // namespace deskmate

#endif  // DESKMATE_GFX_SCREENS_VERTICAL_BARS_LIST_H