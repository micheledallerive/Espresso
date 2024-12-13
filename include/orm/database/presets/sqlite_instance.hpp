#pragma once
#include "orm/utils/stringify.hpp"
#include "utils/string.hpp"
#include <any>
#include <filesystem>
#include <functional>
#include <memory>
#include <sqlite3.h>
#include <string>

namespace espresso::orm {

class SQLiteInstance {
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

    void execute_query(std::string_view query);

    void start_transaction();
    void commit();
    void rollback();

    size_t execute_query(std::string_view query, std::function<void(std::vector<std::any>&)>&& callback);

    class Compiler;
};

class SQLiteInstance::Compiler {
public:
    class Query;
    class Insert;
    class Delete;
};

class SQLiteInstance::Compiler::Query {
private:
    std::string table;
    std::vector<std::string> m_filters;
    std::vector<std::string> m_order_by;
    bool m_count{false};

public:
    explicit Query(std::string table)
        : table(std::move(table))
    {
    }

    SQLiteInstance::Compiler::Query& filter(std::string_view filter)
    {
        m_filters.emplace_back(filter);
        return *this;
    }

    SQLiteInstance::Compiler::Query& order_by(std::string_view order_by)
    {
        m_order_by.emplace_back(order_by);
        return *this;
    }

    SQLiteInstance::Compiler::Query& count()
    {
        m_count = true;
        return *this;
    }

    [[nodiscard]] std::string compile() const
    {
        std::stringstream ss;
        ss << "SELECT ";
        if (m_count) {
            ss << "COUNT(*)";
        }
        else {
            ss << "*";
        }
        ss << " FROM " << table;
        if (!m_filters.empty()) {
            ss << " WHERE ";
            ss << concatenate(m_filters, " AND ");
        }
        if (!m_order_by.empty()) {
            ss << " ORDER BY ";
            ss << concatenate(m_order_by, ", ");
        }
        auto result = ss.str();
        return result;
    }
};

class SQLiteInstance::Compiler::Insert {
private:
    std::string table;
    std::vector<std::string> m_columns;
    std::vector<std::string> m_values;

public:
    explicit Insert(std::string table)
        : table(std::move(table))
    {
    }

    template<typename T>
    SQLiteInstance::Compiler::Insert& insert(std::string_view name, const T& value)
    {
        m_columns.emplace_back(name);
        m_values.push_back(stringify<T>::to_string(value));
        return *this;
    }

    [[nodiscard]] std::string compile() const
    {
        std::stringstream ss;
        ss << "INSERT OR REPLACE INTO " << table;
        ss << " (" << concatenate(m_columns, ", ") << ")";
        ss << " VALUES (\"" << concatenate(m_values, "\", \"") << "\")";
        auto result = ss.str();
        return result;
    }
};

class SQLiteInstance::Compiler::Delete {
private:
    std::string table;
    std::vector<std::string> m_filters;

public:
    explicit Delete(std::string table)
        : table(std::move(table))
    {
    }

    template<typename T>
    SQLiteInstance::Compiler::Delete& filter(std::string_view column_name, const T& field)
    {
        m_filters.push_back(std::string(column_name) + " = \"" + stringify<T>::to_string(field) + "\"");
        return *this;
    }

    [[nodiscard]] std::string compile() const
    {
        std::stringstream ss;
        ss << "DELETE FROM " << table;
        if (!m_filters.empty()) {
            ss << " WHERE ";
            ss << concatenate(m_filters, " AND ");
        }
        auto result = ss.str();
        return result;
    }
};

}// namespace espresso::orm