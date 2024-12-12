#pragma once

#include "utils/string.hpp"
#include <sstream>
#include <string>
#include <vector>

namespace espresso::orm {

class SQLCompiler {
private:
    std::string table;
    std::vector<std::string> m_filters;
    std::vector<std::string> m_order_by;
    bool m_count{false};

public:
    explicit SQLCompiler(std::string table)
        : table(std::move(table))
    {
    }

    SQLCompiler& filter(const std::string& filter)
    {
        m_filters.push_back(filter);
        return *this;
    }

    SQLCompiler& order_by(const std::string& order_by)
    {
        m_order_by.push_back(order_by);
        return *this;
    }

    SQLCompiler& count()
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

}// namespace espresso::orm