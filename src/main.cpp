#include <iostream>

#include <argparse/argparse.hpp>
#include <spdlog/spdlog.h>

#include "config.h"
#include "image/dicom/dicom_uint16_handler.h"
#include "image/dicom/dicom_uint8_handler.h"
#include "image/dicom/dicom_float64_handler.h"
#include "image/tiff/tiff_float32_handler.h"
#include "image/tiff/tiff_uint16_handler.h"
#include "spectrum/spectrum.h"

/**
 * Configures argparser instance for later use
 *
 * Writes error message to stderr if invalid configuration file was provided
 * and help message/version if corresponding flags are present
 *
 * @param program argparse::ArgumentParser instance to be initialized
 * @param argc number of arguments passed to the program
 * @param argv array of argruments passed to the program
 * @return true if configuration was succesfull, false otherwise
 */
bool configureArgparser(argparse::ArgumentParser &program, const int argc, char *argv[]) noexcept {
  program.add_argument<std::string>("-c", "--config")
      .help("path to TOML configuration file")
      .metavar("path/to/config.toml")
      .required();

  program.add_argument<std::string>("-s", "--silent")
      .help("disables program output")
      .flag();

  program.add_argument<std::string>("-v", "--verbose")
      .help("enables additional output")
      .flag();

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception &ex) {
    std::cerr << ex.what() << std::endl;
    std::cerr << program;
    return false;
  }
  return true;
}

int main(const int argc, char *argv[]) {
  argparse::ArgumentParser program("FloodFieldCalculator.exe");
  if (!configureArgparser(program, argc, argv)) {
    return EXIT_FAILURE;
  }

  if (program.get<bool>("--verbose")) {
    spdlog::set_level(spdlog::level::debug);
  }

  if (program.get<bool>("--silent")) {
    spdlog::set_level(spdlog::level::off);
  }

  spdlog::info("Starting FloodFieldCalculator");
  spdlog::debug("Debug messages output enabled");

  const auto &configPath = program.get<std::string>("--config");
  pwn::ffc::config::Config config;
  spdlog::debug("Parsing given config file ({})", configPath);
  try {
    config = pwn::ffc::config::parseConfig(configPath);
  } catch (const std::runtime_error &ex) {
    spdlog::error(ex.what());
    return EXIT_FAILURE;
  }
  spdlog::debug("Successfuly parsed config, {} filter(s) and {} collimator(s) attached",
                config.filters.size(),
                config.collimators.size());

  spdlog::debug("Reading spectrum file contents");
  pwn::ffc::config::Spectrum spectrum;
  try {
    spectrum.readFromCsv(config.system.spectrumTable);
  } catch (const std::runtime_error &ex) {
    spdlog::error(ex.what());
    return EXIT_FAILURE;
  }
  spdlog::debug("Spectrum config successfuly parsed");

  return EXIT_SUCCESS;
}
