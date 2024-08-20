#pragma once
#include "calculator.h"

namespace pwn::ffc::core {
  class AttenuationCalculator final : public Calculator {
    bool m_logarithmize;
    cv::Mat m_rescale_coefficient;

  protected:
    cv::Mat process() override;

  public:
    AttenuationCalculator() = default;
    explicit AttenuationCalculator(bool logarithmize, const cv::Mat &rescale_coefficient);
  };
}
