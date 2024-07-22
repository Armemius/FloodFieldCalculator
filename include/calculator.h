#pragma once

#include "detector/detector.h"
#include "filter/filter.h"
#include "collimator/collimator.h"
#include "frame.h"
#include "signal/signal.h"

#include <vector>

namespace ffc {

class System {
  static System *_instance;

  const Detector *_detector = nullptr;
  const Signal *_signal = nullptr;
  std::vector<Filter *> _filters = std::vector<Filter *>();
  std::vector<Collimator *> _collimators = std::vector<Collimator *>();
  std::vector<Frame> _frames = std::vector<Frame>();

  System();

public:
  static System *get_instance();
  ~System();
  void reset();
  void add_filter(Filter *filter);
  void remove_filters();
  void add_collimator(Collimator *collimator);
  void remove_collimators();
  void set_detector(Detector *detector);
  void remove_detector();
  void set_signal(Signal *signal);
  const Signal &get_signal();
  void remove_signal();
  Frame calculate();

  ffc::Frame calculate(const std::string &material);

  const std::vector<Frame> &get_frames() const;
};

}; // namespace ffc
