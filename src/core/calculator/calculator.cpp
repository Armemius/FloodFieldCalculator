#include "core/calculator/calculator.h"

namespace pwn::ffc::core {
  void Calculator::clear() {
    this->detector_ = nullptr;
    this->filters_.clear();
    this->collimators_.clear();
  }

  Calculator * Calculator::setSpectrum(const config::Spectrum &spectrum) {
    this->spectrum_ = spectrum;
    return this;
  }

  Calculator *Calculator::setDetector(std::unique_ptr<Detector> detector) {
    this->detector_ = std::move(detector);
    return this;
  }

  Calculator *Calculator::addFilter(std::unique_ptr<Filter> filter) {
    this->filters_.push_back(std::move(filter));
    return this;
  }

  Calculator *Calculator::addCollimator(std::unique_ptr<Collimator> collimator) {
    this->collimators_.push_back(std::move(collimator));
    return this;
  }
}
