#pragma once

#include <string>
#include <vector>

namespace pwn::ffc::config {
  /// Struct containing information about material
  struct Material {
    /// Material ID/name
    std::string name;
    /// Number of spectral channels in material
    int channels;
    /// Attenuation coefficients for spectral channels
    std::vector<double> coefficients;
  };
}
