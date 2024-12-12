#pragma once

#include "utils/string.hpp"
#include <sstream>
#include <string>
#include <vector>

namespace espresso::orm {

class SQLCompiler {
public:
    class Query;
    class Insert;
    class Delete;
};

class SQLCompiler::Query {
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

    SQLCompiler::Query& filter(const std::string& filter)
    {
        m_filters.push_back(filter);
        return *this;
    }

    SQLCompiler::Query& order_by(const std::string& order_by)
    {
        m_order_by.push_back(order_by);
        return *this;
    }

    SQLCompiler::Query& count()
    {
        m_count = true;
        return *this;
    }

    std::string compile() const
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

class SQLCompiler::Insert {
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
    SQLCompiler::Insert& insert(std::string_view name, const T& value)
    {
        m_columns.push_back(std::string(name));
        m_values.push_back(stringify<T>::to_string(value));
        return *this;
    }

    std::string compile() const
    {
        std::stringstream ss;
        ss << "INSERT INTO " << table;
        ss << " (" << concatenate(m_columns, ", ") << ")";
        ss << " VALUES (\"" << concatenate(m_values, "\", \"") << "\")";
        auto result = ss.str();
        return result;
    }
};


}// namespace espresso::orm