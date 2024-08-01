#include <utility>

#include "core/filter/filter.h"

namespace pwn::ffc::core {
  Filter::Filter(std::string material) : m_material(std::move(material)) {
  }

  const std::string &Filter::getMaterial() const {
    return this->m_material;
  }
}
