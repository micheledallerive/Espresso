set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

# Add source files
add_library(espresso STATIC)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
target_sources(espresso PRIVATE ${SOURCES})

target_include_directories(espresso PUBLIC ${CMAKE_SOURCE_DIR}/include)
add_library(espresso::espresso ALIAS espresso)