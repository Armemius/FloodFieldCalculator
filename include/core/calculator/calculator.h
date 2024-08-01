#pragma once

#include <memory>
#include <vector>
#include <opencv2/opencv.hpp>

#include "core/collimator/collimator.h"
#include "core/detector/detector.h"
#include "core/filter/filter.h"
#include "spectrum/spectrum.h"

namespace pwn::ffc::core {
  class Calculator {
  protected:
    std::unique_ptr<Detector> detector_ = nullptr;
    config::Spectrum spectrum_{};
    std::vector<std::unique_ptr<Filter> > filters_{};
    std::vector<std::unique_ptr<Collimator> > collimators_{};

  public:
    virtual ~Calculator() = default;

    Calculator() = default;

    void clear();

    Calculator *setSpectrum(const config::Spectrum &spectrum);

    Calculator *setDetector(std::unique_ptr<Detector> detector);

    Calculator *addFilter(std::unique_ptr<Filter> filter);

    Calculator *addCollimator(std::unique_ptr<Collimator> collimator);

    virtual cv::Mat calculateField() = 0;
  };
}
