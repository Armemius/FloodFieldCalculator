#include "signal/attenuation.h"

#include <stdexcept>

ffc::Attenuation::Attenuation(std::vector<double> channel_attenuation)
    : _channel_attenuation(channel_attenuation),
      _channel_count(static_cast<int>(channel_attenuation.size())) {
  if (this->_channel_count == 0) {
    throw std::invalid_argument("Spectral channel attenuation list is empty");
  }
}

ffc::Attenuation::Attenuation(double attenuation_coefficient)
    : _channel_attenuation({attenuation_coefficient}), _channel_count(1) {}

int ffc::Attenuation::channel_count() const { return this->_channel_count; }
