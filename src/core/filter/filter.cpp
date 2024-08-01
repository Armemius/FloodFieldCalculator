#include <utility>

#include "core/filter/filter.h"

namespace pwn::ffc::core {
  Filter::Filter(std::string material) : material_(std::move(material)) {
  }

  const std::string &Filter::getMaterial() const {
    return this->material_;
  }
}
