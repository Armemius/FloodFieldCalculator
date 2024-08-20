#include <utility>

#include "core/filter/filter.h"

namespace pwn::ffc::core {
  Filter::Filter(std::string material, const std::optional<std::string> &id, const double rotation) : m_id(id),
    m_material(std::move(material)),
    m_rotation(rotation) {
  }

  const std::string &Filter::getMaterial() const {
    return this->m_material;
  }

  const std::optional<std::string> &Filter::getId() const {
    return this->m_id;
  }
}
