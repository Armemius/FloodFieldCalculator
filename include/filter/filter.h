#pragma once

#include "measures.h"
#include "signal/attenuation.h"

namespace ffc {

class Filter {
  const Attenuation _attenuation;
  std::string _material;
public:
  virtual double calculate_intersection_length(measures::coords ray_start, measures::coords ray_end) const = 0;
  virtual bool check_intersection(measures::coords ray_start, measures::coords ray_end) const = 0;

  void set_material(std::string material);

  const std::string &get_material();

  Attenuation get_attenuation() const;

  Filter(Attenuation attenuation);
};

} // namespace ffc
