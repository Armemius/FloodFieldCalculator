#pragma once

#include <string>
#include <vector>

namespace pwn::ffc::config {
  struct Material {
    std::string name;
    int channels;
    std::vector<double> coefficients;
  };
}
