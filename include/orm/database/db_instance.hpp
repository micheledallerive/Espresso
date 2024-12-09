#pragma once
#include <any>
#include <functional>
#include <string_view>
#include <vector>

namespace espresso {

class DBInstance {
public:
    DBInstance() = default;
    virtual ~DBInstance() = default;

    virtual void execute_query(std::string_view query) = 0;
    virtual void execute_query(std::string_view query, std::function<void(std::vector<std::any>&)>&& callback) = 0;
};

}// namespace espresso