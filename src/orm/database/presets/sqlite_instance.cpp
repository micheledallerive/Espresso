#include "orm/database/presets/sqlite_instance.hpp"
#include <cassert>

namespace espresso::orm {

SQLiteInstance::SQLiteInstance(const std::string& path)
    : m_db(nullptr, SQLite3Destructor{})
{
    sqlite3* db;
    if (sqlite3_open_v2(path.c_str(), &db, SQLITE_OPEN_READWRITE, nullptr) != SQLITE_OK) {
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

}// namespace espresso::orm