cmake_minimum_required(VERSION 3.12)
project(slight-animation-test)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(slight-animation-test script_tests.cpp)
target_link_libraries(slight-animation-test ${GTEST_LIBRARIES} pthread ${OpenCV_LIBS})
