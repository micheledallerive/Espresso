find_package(SQLite3 REQUIRED)

target_link_libraries(espresso, SQLite3::SQLite3)