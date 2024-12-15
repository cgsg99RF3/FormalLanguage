cmake_minimum_required(VERSION 3.26)
project(LR1Alg)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(tests)

add_executable(LR1Alg main.cpp src/lr1.cpp)