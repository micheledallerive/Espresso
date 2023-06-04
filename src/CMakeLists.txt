# find all cpp files in the current directory recursively
file(GLOB_RECURSE SOURCES *.cpp)

# Create a library target for the Espresso library
add_library(espresso STATIC ${SOURCES})

# Add the source files to the library target
target_sources(espresso PRIVATE ${SOURCES})

# Add the include directories to the library target
target_include_directories(espresso PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Add the library target to the build tree
add_library(espresso::espresso ALIAS espresso)