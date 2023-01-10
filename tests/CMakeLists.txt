cmake_minimum_required(VERSION 3.24)
project(espresso_test)

set(CMAKE_CXX_STANDARD 20)

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
        RouterTests.cpp CookiesTests.cpp HTTPMessageTests.cpp HTTPMethodTests.cpp HTTPRequestTests.cpp HTTPResponseTests.cpp MiddlewareTests.cpp ServerTests.cpp UtilsTests.cpp)

target_link_libraries(
        espresso_test
        espresso_lib
        GTest::gtest_main
)

target_include_directories(
        espresso_test
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

include(GoogleTest)
gtest_discover_tests(espresso_test)
