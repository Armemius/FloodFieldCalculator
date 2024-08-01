#pragma once
#include "calculator.h"

namespace pwn::ffc::core {
  class AttenuationCalculator final : public Calculator {
    bool m_logarithmize;
    double m_rescale_coefficient;

  protected:
    cv::Mat process() override;

  public:
    explicit AttenuationCalculator(bool logarithmize, double rescale_coefficient);
  };
}
