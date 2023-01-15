cmake_minimum_required(VERSION 3.24)
project(espresso_test)

set(CMAKE_CXX_STANDARD 20)

set(TESTS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/)
configure_file(config.h.in config.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
        espresso_test
        RouterTests.cpp CookiesTests.cpp HTTPMessageTests.cpp HTTPMethodTests.cpp HTTPRequestTests.cpp HTTPResponseTests.cpp MiddlewareTests.cpp ServerTests.cpp UtilsTests.cpp RouterTrieTests.cpp RouterTrieNodeTests.cpp)

target_link_libraries(
        espresso_test
        espresso
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(espresso_test)

#exclude all the google libraries from the project