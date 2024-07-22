#include "frame.h"

#include <iostream>
#include <stdexcept>

ffc::Frame::Frame(int width, int height) : _width(width), _height(height) {}

ffc::Frame::Frame(int width, int height, std::vector<double> *data)
    : Frame(width, height) {
  this->set_data(data);
}

ffc::Frame::~Frame() {}

void ffc::Frame::set_data(std::vector<double> *data) {
  if (this->_width * this->_height != data->size()) {
    throw std::invalid_argument("Dimensions and pixel data mismatch");
  }

  this->_data = data;
}

std::vector<double> *ffc::Frame::get_data() const { return this->_data; }

double ffc::Frame::get_pixel(int x, int y) const {
  if (this->_data == nullptr) {
    throw std::logic_error("Data is uninitialized");
  }

  return this->_data->at(y * this->_width + x);
}

void ffc::Frame::set_pixel(int x, int y, double value) {
  if (this->_data == nullptr) {
    throw std::logic_error("Data is uninitialized");
  }

  // std::cout << y * this->_width + x + 1 << "/" << this->_data->size() <<
  // "\n\r";
  this->_data->at(y * this->_width + x) = value;
}

int ffc::Frame::get_width() const { return this->_width; }

int ffc::Frame::get_height() const { return this->_height; }
