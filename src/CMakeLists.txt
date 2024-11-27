set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

# Add source files
add_library(espresso STATIC)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
target_sources(espresso PRIVATE ${SOURCES})

target_include_directories(espresso PUBLIC ${CMAKE_SOURCE_DIR}/include)

# link libmagic-dev
find_library(MAGIC_LIBRARY magic)
target_link_libraries(espresso PUBLIC ${MAGIC_LIBRARY})

add_library(espresso::espresso ALIAS espresso)