cmake_minimum_required(VERSION 3.24)
project(espresso)

set(CMAKE_CXX_STANDARD 20)

# Add the src/ directory as a subdirectory
add_subdirectory(src)

# Install the library
install(TARGETS espresso DESTINATION lib EXPORT espressoConfig DESTINATION lib/cmake/espresso CONFIGURATIONS Release)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/espresso.h DESTINATION include)
