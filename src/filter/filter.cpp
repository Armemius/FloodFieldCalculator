#include "filter/filter.h"

ffc::Filter::Filter(Attenuation attenuation) : _attenuation(attenuation) {}

void ffc::Filter::set_material(std::string material) {
  this->_material = std::move(material);
}

const std::string & ffc::Filter::get_material() {
  return this->_material;
}

ffc::Attenuation ffc::Filter::get_attenuation() const {
  return this->_attenuation;
}
