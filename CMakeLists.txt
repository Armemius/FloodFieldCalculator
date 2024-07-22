cmake_minimum_required(VERSION 3.13)
project(FloodFieldCalculator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

find_package(OpenCV REQUIRED)
find_package(DCMTK REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${DCMTK_INCLUDE_DIRS})
link_directories("C:/Libs/DCMTK/lib/Debug/")

file(GLOB_RECURSE sources CONFIGURE_DEPENDS
        src/*.cpp
        src/*.h
        include/*.h
        libs/*.h
        libs/*.hpp
        libs/*.cpp
)

add_executable(FloodFieldCalculator ${sources})
target_include_directories(FloodFieldCalculator PRIVATE src include libs)
target_link_libraries(FloodFieldCalculator ${OpenCV_LIBS} ${DCMTK_LIBRARIES} Ws2_32 Iphlpapi Netapi32)
