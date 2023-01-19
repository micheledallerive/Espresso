find_package(SQLite3 REQUIRED)

target_link_libraries(espresso PRIVATE SQLite::SQLite3)