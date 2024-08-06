#include <spdlog/spdlog.h>

#include "flood_field_calculator.h"

int main(const int argc, char **argv) {
  try {
    return pwn::ffc::core::FloodFieldCalculator::run(argc, argv);
  } catch (const std::exception &ex) {
    spdlog::error("Unhandled error! {}", ex.what());
    return EXIT_FAILURE;
  }
}
