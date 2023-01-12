cmake_minimum_required(VERSION 3.24)
project(espresso)

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_C_LINK_EXECUTABLE ${CMAKE_CXX_LINK_EXECUTABLE})

# Add the src/ directory as a subdirectory
add_subdirectory(src)

# Install the library target
install(TARGETS espresso
        EXPORT espresso-targets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
install(FILES src/espresso.h DESTINATION include)

