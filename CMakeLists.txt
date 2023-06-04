cmake_minimum_required(VERSION 3.24)
project(espresso)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the src/ directory as a subdirectory
add_subdirectory(src)

add_subdirectory(examples)

option(BUILD_TESTS "Build tests" OFF)
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()

target_include_directories(espresso PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

# Install the library target
install(TARGETS espresso
        EXPORT espresso-targets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/espresso
        DESTINATION include
        FILES_MATCHING PATTERN "*.h")

include(CMakePackageConfigHelpers)
configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/espresso-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/espresso-config.cmake
        INSTALL_DESTINATION lib/cmake/espresso
)

install(EXPORT espresso-targets
        FILE espresso-targets.cmake
        NAMESPACE espresso::
        DESTINATION lib/cmake/espresso)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/espresso-config.cmake
        DESTINATION lib/cmake/espresso)

export(EXPORT espresso-targets
        FILE ${CMAKE_CURRENT_BINARY_DIR}/espresso-targets.cmake
        NAMESPACE espresso::)