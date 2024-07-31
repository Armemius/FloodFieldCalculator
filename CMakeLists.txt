cmake_minimum_required(VERSION 3.28.1)
project(FloodFieldCalculator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(argparse)
find_package(spdlog)
find_package(toml11)
find_package(OpenCV)
find_package(DCMTK)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS
        src/*.cpp
        src/*.h
        include/*.h
)

add_executable(FloodFieldCalculator ${sources})
target_include_directories(FloodFieldCalculator PRIVATE src include libs)
target_link_libraries(FloodFieldCalculator PRIVATE
        dcmtk::dcmtk
        opencv::opencv
        toml11::toml11
        spdlog::spdlog
        argparse::argparse
)

