#include <cstdlib>
#include <spdlog/spdlog.h>

#include "flood_field_calculator.h"

// TODO README.md
// TODO Documentation

int main(const int argc, char **argv) {
  try {
    return pwn::ffc::core::FloodFieldCalculator(argc, argv).run();
  } catch (const std::exception &ex) {
    spdlog::error("Unhandled error! {}", ex.what());
    return EXIT_FAILURE;
  }
}
