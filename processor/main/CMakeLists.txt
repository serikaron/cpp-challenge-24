cmake_minimum_required(VERSION 3.27.8)
project(main)

set(CMAKE_CXX_STANDARD 26)

add_executable(processor main.cpp)
target_link_libraries(processor PRIVATE core)
