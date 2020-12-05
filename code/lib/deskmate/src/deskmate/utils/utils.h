#ifndef DESKMATE_UTILS_UTILS_H
#define DESKMATE_UTILS_UTILS_H

#include <sstream>

namespace deskmate {
namespace utils {

template <typename FloatingPoint>
std::string to_fixed(FloatingPoint value, const int decimal_places) {
  std::ostringstream oss;
  oss.precision(1);
  oss << std::fixed << value;
  return oss.str();
}

}  // namespace utils
}  // namespace deskmate
#endif  // DESKMATE_UTILS_UTILS_H