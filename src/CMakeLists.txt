# Collect all the source files in the directory
file(GLOB_RECURSE SOURCES "*.cpp")

# Collect all the header files in the directory
file(GLOB_RECURSE HEADERS "*.h")

# Create a library target for the Espresso library
add_library(espresso_lib STATIC ${SOURCES})

# Add the source files to the library target
target_sources(espresso_lib PRIVATE ${SOURCES} ${HEADERS})

# Include the headers directory for the library target
target_include_directories(espresso_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

