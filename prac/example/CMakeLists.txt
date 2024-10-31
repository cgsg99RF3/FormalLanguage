cmake_minimum_required(VERSION 3.22)
project(example)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(SOURCE_FILES main.cpp ../src/prac1.cpp)

include_directories(../include)

add_executable(example ${SOURCE_FILES})