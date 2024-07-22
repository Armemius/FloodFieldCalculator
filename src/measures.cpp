#include "measures.h"

double ffc::measures::distance(const coords &from, const coords &to) {
  return sqrt((from.x - to.x) * (from.x - to.x) +
              (from.y - to.y) * (from.y - to.y) +
              (from.z - to.z) * (from.z - to.z));
}

std::ostream &ffc::measures::operator<<(std::ostream &os,
                                        const coords &coords) {
  os << "[X: " << coords.x << "; Y: " << coords.y << "; Z: " << coords.z << "]";
  return os;
}

template <typename T>
std::ostream &ffc::measures::operator<<(std::ostream &os,
                                        const dimensions<T> &dimensions) {
  os << "[Width: " << dimensions.width << "; Height: " << dimensions.height
     << "]";
  return os;
}
