#include "core/calculator/calculator.h"

#include <spdlog/spdlog.h>

namespace pwn::ffc::core {
  void Calculator::check() const {
    if (this->m_detector == nullptr) {
      throw std::runtime_error("Detector is not attached to calculator");
    }
    for (const auto &filter : m_filters) {
      try {
        static_cast<void>(this->m_spectrum.getMaterialCoefficients(filter->getMaterial()));
      } catch (...) {
        spdlog::error("Material {} is not found in current spectrum config", filter->getMaterial());
      }
    }
    if (this->m_filters.empty() && this->m_collimators.empty()) {
      spdlog::warn("Calculating field without any filters and collimators");
    }
  }

  void Calculator::clear() {
    this->m_detector = nullptr;
    this->m_filters.clear();
    this->m_collimators.clear();
    this->m_spectrum = {};
  }

  Calculator * Calculator::setSpectrum(const config::Spectrum &spectrum) {
    this->m_spectrum = spectrum;
    return this;
  }

  Calculator *Calculator::setDetector(std::unique_ptr<Detector> detector) {
    this->m_detector = std::move(detector);
    return this;
  }

  Calculator *Calculator::addFilter(std::unique_ptr<Filter> filter) {
    this->m_filters.push_back(std::move(filter));
    return this;
  }

  Calculator *Calculator::addCollimator(std::unique_ptr<Collimator> collimator) {
    this->m_collimators.push_back(std::move(collimator));
    return this;
  }

  cv::Mat Calculator::calculateField() {
    this->check();
    return this->process();
  }
}
