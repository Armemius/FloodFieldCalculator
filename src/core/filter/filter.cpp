#include <utility>

#include "core/filter/filter.h"

namespace pwn::ffc::core {
  Filter::Filter(std::string material, const std::optional<std::string> &id) : m_id(id), m_material(std::move(material)) {
  }

  const std::string &Filter::getMaterial() const {
    return this->m_material;
  }

  const std::optional<std::string> & Filter::getId() const {
    return this->m_id;
  }
}
