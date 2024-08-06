#pragma once

#include <argparse/argparse.hpp>

#include "config.h"

namespace pwn::ffc::core {
  class FloodFieldCalculator {
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
    static bool configureArgparser(argparse::ArgumentParser &program, int argc, char *argv[]) noexcept;

    FloodFieldCalculator() = default;

  public:
    /**
     * Starts FloodFieldCalculator execution
     *
     * @param argc number of arguments passed to the program
     * @param argv array of argruments passed to the program
     * @return program exit status
     */
    static int run(int argc, char *argv[]);

    static int run(config::Config config);
  };
}
