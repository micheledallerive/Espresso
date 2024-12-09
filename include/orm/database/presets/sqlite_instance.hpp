#pragma once
#include "orm/utils.hpp"
#include "orm/database/db_instance.hpp"
#include <any>
#include <filesystem>
#include <functional>
#include <memory>
#include <sqlite3.h>
#include <string>

namespace espresso::orm {

class SQLiteInstance : public DBInstance {
private:
    struct SQLite3Destructor {
        void operator()(sqlite3* db) const
        {
            sqlite3_close(db);
        }
    };
    struct StatementDestructor {
        void operator()(sqlite3_stmt* stmt) const
        {
            sqlite3_finalize(stmt);
        }
    };

    std::unique_ptr<sqlite3, SQLite3Destructor> m_db;

    bool m_in_transaction{false};

    std::unique_ptr<sqlite3_stmt, StatementDestructor> generate_stmt(std::string_view query);

public:
    explicit SQLiteInstance(const std::filesystem::path& path);
    ~SQLiteInstance() = default;

    void execute_query(std::string_view query) override;

    void start_transaction();
    void commit();
    void rollback();

    void execute_query(std::string_view query, std::function<void(std::vector<std::any>&)>&& callback) override;
};

}// namespace espresso::orm