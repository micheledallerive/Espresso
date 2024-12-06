#pragma once
#include "orm/utils.hpp"
#include <filesystem>
#include <functional>
#include <memory>
#include <sqlite3.h>
#include <string>

namespace espresso::orm {

class SQLiteInstance {
private:
    struct ConversionHelper {
        template<typename T>
        static T convert_column_value(sqlite3_stmt* stmt, int column);

        template<>
        int convert_column_value<int>(sqlite3_stmt* stmt, int column)
        {
            return sqlite3_column_int(stmt, column);
        }

        template<>
        std::string convert_column_value<std::string>(sqlite3_stmt* stmt, int column)
        {
            return {reinterpret_cast<const char*>(sqlite3_column_text(stmt, column))};
        }

        template<>
        float convert_column_value<float>(sqlite3_stmt* stmt, int column)
        {
            return static_cast<float>(sqlite3_column_double(stmt, column));
        }

        template<>
        double convert_column_value<double>(sqlite3_stmt* stmt, int column)
        {
            return sqlite3_column_double(stmt, column);
        }

        template<>
        bool convert_column_value<bool>(sqlite3_stmt* stmt, int column)
        {
            return sqlite3_column_int(stmt, column) != 0;
        }

        template<>
        char convert_column_value<char>(sqlite3_stmt* stmt, int column)
        {
            return *reinterpret_cast<const char*>(sqlite3_column_text(stmt, column));
        }

        template<>
        long convert_column_value<long>(sqlite3_stmt* stmt, int column)
        {
            if constexpr (sizeof(long) == 4) {
                return sqlite3_column_int(stmt, column);
            }
            else {
                return sqlite3_column_int64(stmt, column);
            }
        }

        template<>
        long long convert_column_value<long long>(sqlite3_stmt* stmt, int column)
        {
            return sqlite3_column_int64(stmt, column);
        }
    };

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

    template<typename... ReturnTypes>
    void execute_query(std::string_view query, std::function<void(std::tuple<ReturnTypes...>)>&& callback)
    {
        auto stmt = generate_stmt(query);

        auto ret = sqlite3_step(stmt.get());
        while (ret == SQLITE_ROW) {

            std::tuple<ReturnTypes...> result;

            [&result, &stmt]<size_t... _i>(std::index_sequence<_i...>) {
                ((std::get<_i>(result) = ConversionHelper::convert_column_value<ReturnTypes>(stmt.get(), _i)), ...);
            }(std::make_index_sequence<sizeof...(ReturnTypes)>{});

            callback(result);
            ret = sqlite3_step(stmt.get());
        }
        if (ret != SQLITE_DONE) {
            throw std::runtime_error("Failed to execute query");
        }
    }
    template<typename Callable>
        requires(!is_specialization_of_v<Callable, std::function>)
    void execute_query(std::string_view query, Callable&& callback)
    {
        execute_query(query, std::function(callback));
    }
};

}// namespace espresso::orm