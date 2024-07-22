#pragma once

#include <vector>

namespace ffc {

class Attenuation {
  friend class Signal;

  const int _channel_count;
  const std::vector<double> _channel_attenuation;

public:
  Attenuation(std::vector<double> channel_attenuation);
  Attenuation(double attenuation_coefficient);
  int channel_count() const;
};

} // namespace ffc
