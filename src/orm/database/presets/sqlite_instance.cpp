#include "orm/database/presets/sqlite_instance.hpp"
#include <cassert>

namespace espresso::orm {

SQLiteInstance::SQLiteInstance(const std::filesystem::path& path)
    : m_db(nullptr, SQLite3Destructor{})
{
    sqlite3* db;
    if (sqlite3_open(path.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error("Failed to open database");
    }
    m_db.reset(db);
}
void SQLiteInstance::start_transaction()
{
    assert(!m_in_transaction);
    if (sqlite3_exec(m_db.get(), "BEGIN", nullptr, nullptr, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to start transaction");
    }

    m_in_transaction = true;
}
void SQLiteInstance::rollback()
{
    assert(m_in_transaction);
    if (sqlite3_exec(m_db.get(), "ROLLBACK", nullptr, nullptr, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to rollback transaction");
    }

    m_in_transaction = false;
}
void SQLiteInstance::commit()
{
    assert(m_in_transaction);
    if (sqlite3_exec(m_db.get(), "COMMIT", nullptr, nullptr, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to commit transaction");
    }

    m_in_transaction = false;
}
std::unique_ptr<sqlite3_stmt, SQLiteInstance::StatementDestructor> SQLiteInstance::generate_stmt(std::string_view query)
{
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db.get(),
                           query.data(),
                           static_cast<int>(query.size()),
                           &stmt,
                           nullptr)
        != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement");
    }
    return {stmt, StatementDestructor{}};
}
void SQLiteInstance::execute_query(std::string_view query)
{
    auto stmt = generate_stmt(query);

    auto ret = sqlite3_step(stmt.get());
    if (ret != SQLITE_DONE) {
        throw std::runtime_error("Failed to execute query");
    }
}
size_t SQLiteInstance::execute_query(std::string_view query, std::function<void(std::vector<std::any>&)>&& callback)
{
    auto stmt = generate_stmt(query);

    size_t cnt = 0;
    auto ret = sqlite3_step(stmt.get());
    while (ret == SQLITE_ROW) {
        int result_size = sqlite3_column_count(stmt.get());
        std::vector<std::any> result;result.reserve(result_size);
        for (int i = 0; i < result_size; ++i) {
            switch (sqlite3_column_type(stmt.get(), i)) {
            case SQLITE_INTEGER:
                result.emplace_back(sqlite3_column_int(stmt.get(), i));
                break;
            case SQLITE_FLOAT:
                result.emplace_back(sqlite3_column_double(stmt.get(), i));
                break;
            case SQLITE_TEXT:
                result.emplace_back(std::string{reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), i))});
                break;
            case SQLITE_BLOB:
                result.emplace_back(std::vector<char>{reinterpret_cast<const char*>(sqlite3_column_blob(stmt.get(), i)),
                                                      reinterpret_cast<const char*>(sqlite3_column_blob(stmt.get(), i)) + sqlite3_column_bytes(stmt.get(), i)});
                break;
            case SQLITE_NULL:
                result.emplace_back();
                break;
            default:
                throw std::runtime_error("Unknown column type");
            }
        }

        ++cnt;
        callback(result);
        ret = sqlite3_step(stmt.get());
    }
    if (ret != SQLITE_DONE) {
        throw std::runtime_error("Failed to execute query");
    }
    return cnt;
}

}// namespace espresso::orm