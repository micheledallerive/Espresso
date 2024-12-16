#pragma once
#include "orm/utils/stringify.hpp"
#include "orm/utils/types.hpp"
#include "utils/string.hpp"
#include "utils/tuple.hpp"
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
    template<typename... Types>
    size_t execute_query(std::string_view query, std::function<void(const Tuple<Types...>&)>&& callback)
    {
        auto stmt = generate_stmt(query);

        size_t cnt = 0;
        auto ret = sqlite3_step(stmt.get());
        while (ret == SQLITE_ROW) {
            Tuple<Types...> result;
            [&]<std::size_t... I>(std::index_sequence<I...>) {
                ((std::get<I>(result) = SQLiteInstance::Parser<rfl::tuple_element_t<I, Tuple<Types...>>>::parse(stmt.get(), I)), ...);
            }(make_index_sequence_skip_compound<Types...>{});

            ++cnt;
            callback(result);
            ret = sqlite3_step(stmt.get());
        }
        if (ret != SQLITE_DONE) {
            throw std::runtime_error("Failed to execute query");
        }
        return cnt;
    }

    void start_transaction();
    void commit();
    void rollback();

    class Compiler;
    template<typename T>
    struct Parser;
};

class SQLiteInstance::Compiler {
public:
    class Query;
    class Insert;
    class Delete;
};

class SQLiteInstance::Compiler::Query {
private:
    friend class SQLiteInstance::Compiler::Insert;
    friend class SQLiteInstance::Compiler::Delete;

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
        m_values.push_back(quote_stringify<T>::to_string(value));
        return *this;
    }

    [[nodiscard]] std::string compile() const
    {
        std::stringstream ss;
        ss << "INSERT OR REPLACE INTO " << table;
        ss << " (" << concatenate(m_columns, ", ") << ")";
        ss << " VALUES (" << concatenate(m_values, ", ") << ")";
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

    explicit Delete(SQLiteInstance::Compiler::Query&& query) : table(std::move(query.table)), m_filters(std::move(query.m_filters)) {}

    template<typename T>
    SQLiteInstance::Compiler::Delete& filter(std::string_view column_name, const T& field)
    {
        std::string rhs = quote_stringify<T>::to_string(field);
        std::string op = " = ";
        if (rhs == "NULL") op = " IS ";
        m_filters.push_back(std::string(column_name) + op + rhs);
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

template<>
struct SQLiteInstance::Parser<std::string> {
    static std::string parse(sqlite3_stmt* stmt, size_t i)
    {
        return std::string{reinterpret_cast<const char*>(sqlite3_column_text(stmt, i))};
    }
};

template<>
struct SQLiteInstance::Parser<int> {
    static int parse(sqlite3_stmt* stmt, size_t i)
    {
        return sqlite3_column_int(stmt, i);
    }
};

template<>
struct SQLiteInstance::Parser<double> {
    static double parse(sqlite3_stmt* stmt, size_t i)
    {
        return sqlite3_column_double(stmt, i);
    }
};

template<typename T>
struct SQLiteInstance::Parser<std::optional<T>> {
    static std::optional<T> parse(sqlite3_stmt* stmt, size_t i)
    {
        if (sqlite3_column_type(stmt, i) == SQLITE_NULL) {
            return std::nullopt;
        }
        return Parser<T>::parse(stmt, i);
    }
};

template<typename T>
    requires requires { typename T::Compound; }
struct SQLiteInstance::Parser<T> {
    static T parse(sqlite3_stmt* stmt, size_t i)
    {
        typename T::Compound key;
        [&]<std::size_t... j>(std::index_sequence<j...>) {
            ((std::get<j>(key) = Parser<rfl::tuple_element_t<j, typename T::Compound>>::parse(stmt, i + j)), ...);
        }(std::make_index_sequence<rfl::tuple_size_v<typename T::Compound>>{});
        return T(key);
    }
};

}// namespace espresso::orm