cmake_minimum_required(VERSION 3.12)
project(slight-animation)

set(CMAKE_CXX_STANDARD 11)

add_executable(slight-animation main.cpp)

find_package(OpenCV REQUIRED)
target_link_libraries(slight-animation ${OpenCV_LIBS})