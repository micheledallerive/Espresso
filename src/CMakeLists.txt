add_library(espresso STATIC)

# Add source files
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
target_sources(espresso PRIVATE ${SOURCES})

# Include headers
target_include_directories(espresso PUBLIC ${CMAKE_SOURCE_DIR}/include)