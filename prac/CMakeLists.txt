cmake_minimum_required(VERSION 3.10)
project(Formal_language_prac)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Building with code coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

include(FetchContent)
FetchContent_Declare(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(gtest)

enable_testing()
include_directories(include)

add_executable(MyTests src/tests.cpp src/prac1.cpp)
target_link_libraries(MyTests gtest gtest_main)

add_test(NAME MyTests COMMAND MyTests)

add_custom_target(coverage
        COMMAND ctest
        COMMAND gcovr -r ${PROJECT_SOURCE_DIR} --html --html-details -o coverage.html
        DEPENDS MyTests
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
