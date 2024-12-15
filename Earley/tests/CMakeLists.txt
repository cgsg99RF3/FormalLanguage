# tests/CMakeLists.txt
add_executable(MyTests test_earley.cpp) # Замените test_my_library.cpp на ваши тестовые файлы

# Связываем с библиотекой и gtest
target_link_libraries(MyTests MyLibrary ${GTEST_LIBRARIES} pthread)

# Добавляем тесты
add_test(NAME MyTests COMMAND MyTests)

