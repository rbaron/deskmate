#ifndef DESKMATE_UTILS_UTILS_H
#define DESKMATE_UTILS_UTILS_H

#include <sstream>

namespace deskmate {
namespace utils {

template <typename FloatingPoint>
std::string to_fixed(FloatingPoint value, const int precision) {
  std::ostringstream oss;
  oss.precision(precision);
  oss << std::fixed << value;
  return oss.str();
}

}  // namespace utils
}  // namespace deskmate
#endif  // DESKMATE_UTILS_UTILS_H