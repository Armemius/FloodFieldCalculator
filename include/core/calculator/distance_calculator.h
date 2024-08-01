#pragma once
#include "calculator.h"

namespace pwn::ffc::core {
  class DistanceCalculator : public Calculator {
  protected:
    cv::Mat process() override;
  };
}
