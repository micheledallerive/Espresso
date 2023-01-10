cmake_minimum_required(VERSION 3.24)
project(espresso)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
add_test(NAME espresso_tests COMMAND espresso_test)
