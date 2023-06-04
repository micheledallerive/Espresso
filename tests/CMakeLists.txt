cmake_minimum_required(VERSION 3.24)
project(espresso_test)

set(CMAKE_CXX_STANDARD 20)

set(TESTS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/)
configure_file(config.h.in config.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(GTest REQUIRED)

# discover all tests .cpp files with tests
file(GLOB_RECURSE TESTS_FILES ${TESTS_PATH} "*.cpp")

add_executable(
        espresso_test
        ${TESTS_FILES}
)
target_link_libraries(espresso_test PRIVATE espresso GTest::GTest GTest::Main)

add_dependencies(espresso_test espresso)

include(GoogleTest)
gtest_discover_tests(espresso_test)

#exclude all the google libraries from the project