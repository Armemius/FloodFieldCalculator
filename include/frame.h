#pragma once

#include <vector>

namespace ffc {

class Frame {
  const int _width;
  const int _height;
  std::vector<double> *_data = nullptr;

public:
  Frame(int width, int height);
  Frame(int width, int height, std::vector<double> *data);
  ~Frame();
  void set_data(std::vector<double> *data);
  std::vector<double> *get_data() const;
  double get_pixel(int x, int y) const;
  void set_pixel(int x, int y, double value);
  int get_width() const;
  int get_height() const;
};

} // namespace ffc
