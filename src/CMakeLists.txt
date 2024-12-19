# Add source files
add_library(espresso STATIC)

# External libraries linking
find_library(MAGIC_LIBRARY magic)
target_link_libraries(espresso PUBLIC ${MAGIC_LIBRARY})

espresso_link_external(espresso)
# End external libraries linking

# Set compile options
#target_compile_options(espresso PRIVATE -Wall -Wextra -Wpedantic -Werror)
#set_target_properties(espresso PROPERTIES COMPILE_FLAGS "-Wall -Wextra -Wpedantic -Werror")

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
target_sources(espresso PRIVATE ${SOURCES})

target_include_directories(espresso PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_library(espresso::espresso ALIAS espresso)