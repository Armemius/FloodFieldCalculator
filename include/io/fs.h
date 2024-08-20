#pragma once

#include <filesystem>

namespace pwn::io {

    bool file_exists(const std::string &file);

    std::string get_file_extension(const std::string &file);

    bool directory_exists(const std::string &directory);

    bool create_directory(const std::string &directory);

    bool clear_directory_contents(const std::string &directory);

    bool prepare_output_directory(const std::string &directory);
}
