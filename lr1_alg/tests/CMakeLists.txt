# tests/CMakeLists.txt
add_executable(MyTests test_lr1.cpp)

target_link_libraries(MyTests MyLibrary ${GTEST_LIBRARIES} pthread)
add_test(NAME MyTests COMMAND MyTests)

