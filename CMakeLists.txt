cmake_minimum_required(VERSION 3.28.1)
project(FloodFieldCalculator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(rapidcsv REQUIRED)
find_package(argparse REQUIRED)
find_package(spdlog REQUIRED)
find_package(toml11 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(DCMTK REQUIRED)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS
        src/*.cpp
        src/*.h
        include/*.h
)

add_library(ffc STATIC ${sources})
add_executable(FloodFieldCalculator ${sources})

target_include_directories(ffc
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        $<INSTALL_INTERFACE:include/pwn/ffc>
        PRIVATE src
)
target_include_directories(FloodFieldCalculator PRIVATE src include)

target_link_libraries(FloodFieldCalculator
        PUBLIC
        dcmtk::dcmtk
        opencv::opencv
        spdlog::spdlog
        argparse::argparse
        toml11::toml11
        rapidcsv::rapidcsv
)

target_link_libraries(ffc
        PUBLIC
        dcmtk::dcmtk
        opencv::opencv
        spdlog::spdlog
        argparse::argparse
        toml11::toml11
        rapidcsv::rapidcsv
)

include(GNUInstallDirs)
install(TARGETS ffc
        EXPORT FloodFieldCalculatorTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pwn/ffc)

install(EXPORT FloodFieldCalculatorTargets
        FILE FloodFieldCalculatorTargets.cmake
        NAMESPACE pwn::ffc::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FloodFieldCalculator
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/FloodFieldCalculatorConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

configure_file(FloodFieldCalculatorConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/FloodFieldCalculatorConfig.cmake
        @ONLY
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/FloodFieldCalculatorConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/FloodFieldCalculatorConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FloodFieldCalculator
)
