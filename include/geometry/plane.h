#pragma once

namespace pwn::ffc::geometry {
  /// Structs representing size of plane dimensions
  template <typename T>
  struct Plane {
    T width;
    T height;
  };
}