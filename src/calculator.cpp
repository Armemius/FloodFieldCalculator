#include "calculator.h"
#include <iostream>

ffc::System *ffc::System::_instance = nullptr;

ffc::System::System() {}

ffc::System::~System() { this->reset(); }

ffc::System *ffc::System::get_instance() {
  if (ffc::System::_instance == nullptr) {
    ffc::System::_instance = new System();
  }
  return ffc::System::_instance;
}

void ffc::System::reset() {
  this->remove_detector();
  this->remove_filters();
  this->remove_collimators();
}

void ffc::System::add_filter(Filter *filter) {
  if (this->_signal->channel_count() !=
      filter->get_attenuation().channel_count()) {
    throw std::logic_error("Spectral channels count mismatch");
  }

  this->_filters.push_back(filter);
}

void ffc::System::remove_filters() {
  for (size_t it = 0; it < this->_filters.size(); ++it) {
    if (this->_filters[it] != nullptr) {
      delete this->_filters[it];
    }
  }

  this->_filters.clear();
}

void ffc::System::set_detector(Detector *detector) {
  this->remove_detector();
  this->_detector = detector;
}

void ffc::System::remove_detector() {
  if (this->_detector != nullptr) {
    delete this->_detector;
  }
  this->_detector = nullptr;
}

void ffc::System::set_signal(Signal *signal) {
  this->remove_signal();
  this->_signal = signal;
}

void ffc::System::remove_signal() {
  if (this->_signal != nullptr) {
    delete this->_signal;
  }
  this->_signal = nullptr;
}

void ffc::System::add_collimator(Collimator *collimator) {
  this->_collimators.push_back(collimator);
}

void ffc::System::remove_collimators() {
  for (size_t it = 0; it < this->_collimators.size(); ++it) {
    if (this->_collimators[it] != nullptr) {
      delete this->_collimators[it];
    }
  }

  this->_collimators.clear();
}

ffc::Frame ffc::System::calculate() {
  if (this->_detector == nullptr) {
    throw std::logic_error("System has no detectors");
  }
  if (this->_signal == nullptr) {
    throw std::logic_error("System signal is not defined");
  }
  const auto DETECTOR_RESOLUTION = this->_detector->get_resolution();
  Frame result = Frame(DETECTOR_RESOLUTION.width, DETECTOR_RESOLUTION.height,
                       new std::vector<double>(DETECTOR_RESOLUTION.width *
                                               DETECTOR_RESOLUTION.height));

  for (size_t jt = 0; jt < DETECTOR_RESOLUTION.height; ++jt) {
    for (size_t it = 0; it < DETECTOR_RESOLUTION.width; ++it) {
      static const measures::coords ray_start = {0.0, 0.0, 0.0};
      const measures::coords ray_end =
          this->_detector->calculate_coords_of_pixel(static_cast<int>(it),
                                                     static_cast<int>(jt));

      double res = 1;

      for (const auto &filter : _filters) {
        if (filter->check_intersection(ray_start, ray_end)) {
          const double intersection_length =
              filter->calculate_intersection_length(ray_start, ray_end);
          res *= this->_signal->calculate_attenuation(
              intersection_length, filter->get_attenuation());
        }
      }

      for (auto &collimator : _collimators) {
        if (collimator->check_intersection(ray_start, ray_end)) {
          res = 0;
          break;
        }
      }

      result.set_pixel(static_cast<int>(it), static_cast<int>(jt), res);
    }
  }

  this->_frames.push_back(result);
  return result;
}

ffc::Frame ffc::System::calculate(const std::string &material) {
  if (this->_detector == nullptr) {
    throw std::logic_error("System has no detectors");
  }
  if (this->_signal == nullptr) {
    throw std::logic_error("System signal is not defined");
  }
  const auto DETECTOR_RESOLUTION = this->_detector->get_resolution();
  Frame result = Frame(DETECTOR_RESOLUTION.width, DETECTOR_RESOLUTION.height,
                       new std::vector<double>(DETECTOR_RESOLUTION.width *
                                               DETECTOR_RESOLUTION.height));

  for (size_t jt = 0; jt < DETECTOR_RESOLUTION.height; ++jt) {
    for (size_t it = 0; it < DETECTOR_RESOLUTION.width; ++it) {
      static const measures::coords ray_start = {0.0, 0.0, 0.0};
      const measures::coords ray_end =
          this->_detector->calculate_coords_of_pixel(static_cast<int>(it),
                                                     static_cast<int>(jt));

      double res = 0;

      for (const auto &filter : _filters) {
        if (filter->get_material() != material) {
          continue;
        }
        if (filter->check_intersection(ray_start, ray_end)) {
          const double intersection_length =
              filter->calculate_intersection_length(ray_start, ray_end);
          res += intersection_length;
        }
      }

      result.set_pixel(static_cast<int>(it), static_cast<int>(jt), res);
    }
  }

  this->_frames.push_back(result);
  return result;
}

const std::vector<ffc::Frame> &ffc::System::get_frames() const {
  return this->_frames;
}

const ffc::Signal &ffc::System::get_signal() { return *this->_signal; }
