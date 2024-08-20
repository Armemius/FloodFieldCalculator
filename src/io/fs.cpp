#include "io/fs.h"

bool pwn::io::file_exists(const std::string &file) {
    return std::filesystem::exists(file) && !std::filesystem::is_directory(file);
}

std::string pwn::io::get_file_extension(const std::string &file) {
    return std::filesystem::path(file).extension().string();
}

bool pwn::io::directory_exists(const std::string &directory) {
    return std::filesystem::exists(directory) && std::filesystem::is_directory(directory);
}

bool pwn::io::create_directory(const std::string &directory) {
    return std::filesystem::create_directories(directory);
}

bool pwn::io::clear_directory_contents(const std::string &directory) {
    try {
        for (const auto &entry: std::filesystem::directory_iterator(directory)) {
            remove_all(entry.path());
        }
        return true;
    } catch (const std::filesystem::filesystem_error &e) {
        return false;
    }
}

bool pwn::io::prepare_output_directory(const std::string &directory) {
    if (!directory_exists(directory)) {
        return create_directory(directory);
    }
    return clear_directory_contents(directory);
}
