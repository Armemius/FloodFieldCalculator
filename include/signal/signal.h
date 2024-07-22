#pragma once

#include "signal/attenuation.h"
#include <vector>

namespace ffc {

class Signal {
  const int _channel_count;
  const std::vector<double> _channel_forces;

public:
  Signal(std::vector<double> channel_forces);
  Signal();
  double calculate_attenuation(double length,
                               const Attenuation &attenuation) const;
  int channel_count() const;
};

} // namespace ffc
