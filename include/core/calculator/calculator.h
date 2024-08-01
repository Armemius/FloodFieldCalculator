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
    void check() const;

  protected:
    std::unique_ptr<Detector> m_detector = nullptr;
    config::Spectrum m_spectrum = {};
    std::vector<std::unique_ptr<Filter> > m_filters;
    std::vector<std::unique_ptr<Collimator> > m_collimators;

    virtual cv::Mat process() = 0;

  public:
    virtual ~Calculator() = default;

    Calculator() = default;

    virtual void clear();

    virtual Calculator *setSpectrum(const config::Spectrum &spectrum);

    virtual Calculator *setDetector(std::unique_ptr<Detector> detector);

    virtual Calculator *addFilter(std::unique_ptr<Filter> filter);

    virtual Calculator *addCollimator(std::unique_ptr<Collimator> collimator);

    cv::Mat calculateField();
  };
}
