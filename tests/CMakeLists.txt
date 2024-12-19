include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/b514bdc898e2951020cbdca1304b75f5950d1f59.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.cpp)
foreach (test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(test_${test_name} ${test_source})

    target_link_libraries(test_${test_name} PRIVATE GTest::gtest_main)
    target_include_directories(test_${test_name} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    espresso_link_external(test_${test_name})

    add_test(NAME test_${test_name} COMMAND test_${test_name})
endforeach ()