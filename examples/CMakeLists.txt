file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

foreach(SOURCE ${SOURCES})
    get_filename_component(EXAMPLE_NAME ${SOURCE} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${SOURCE})
    target_link_libraries(${EXAMPLE_NAME} PRIVATE espresso::espresso)
    target_link_libraries(${EXAMPLE_NAME} PRIVATE nlohmann_json::nlohmann_json)
    target_include_directories(${EXAMPLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
endforeach()