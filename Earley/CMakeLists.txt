cmake_minimum_required(VERSION 3.26)
project(EarleyAlg)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src)

# Добавьте gtest
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Укажите директорию с тестами
add_subdirectory(tests)

add_executable(Earley main.cpp src/earley.cpp)