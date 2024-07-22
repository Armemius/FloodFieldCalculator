#include "signal/signal.h"
#include <stdexcept>

ffc::Signal::Signal(std::vector<double> channel_forces)
    : _channel_forces(channel_forces),
      _channel_count(static_cast<int>(channel_forces.size())) {
  if (this->_channel_count == 0) {
    throw std::invalid_argument("Spectral channel forces list is empty");
  }
}

ffc::Signal::Signal() : _channel_forces({1.0}), _channel_count(1) {}

double
ffc::Signal::calculate_attenuation(double length,
                                   const Attenuation &attenuation) const {
  if (attenuation._channel_count != this->_channel_count) {
    throw std::invalid_argument("Channel count mismatch");
  }

  double res = 0;
  for (size_t it = 0; it < this->_channel_count; ++it) {
    res += _channel_forces[it] *
           exp(-attenuation._channel_attenuation[it] * length);
  }

  return res;
}

int ffc::Signal::channel_count() const { return this->_channel_count; }
