#include <iostream>

#include <argparse/argparse.hpp>
#include <spdlog/spdlog.h>

/**
 * Configures argparser instance for later use
 *
 * @param program argparse::ArgumentParser instance to be initialized
 * @param argc number of arguments passed to the program
 * @param argv array of argruments passed to the program
 * @return true if configuration was succesfull, false otherwise
 */
bool configure_argparser(argparse::ArgumentParser &program, const int argc, char *argv[]) {
  std::string config_path;
  program.add_argument("-c", "--config")
      .help("path to TOML configuration file")
      .required()
      .metavar("path/to/config.toml")
      .store_into(config_path);

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
  if (!configure_argparser(program, argc, argv)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
