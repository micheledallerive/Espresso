#pragma once

#include <any>
#include <map>
#include <string>

namespace espresso {

class CustomData {
private:
    std::map<std::string, std::any> m_data;

public:
    CustomData() = default;

    template<typename T>
    void insert(const std::string& key, T value)
    {
        m_data[key] = std::move(value);
    }

    bool has(const std::string& key) const
    {
        return m_data.find(key) != m_data.end();
    }

    template<typename T>
    bool has_as(const std::string& key) const
    {
        return has(key) && m_data.at(key).type() == typeid(T);
    }

    template<typename T>
    const T& at(const std::string& key) const
    {
        return std::any_cast<const T&>(m_data.at(key));
    }

    template<typename T>
    T& get(const std::string& key)
    {
        if (!has(key)) {
            insert(key, T{});
        }
        return std::any_cast<T&>(m_data.at(key));
    }

    std::any operator[](const std::string& key)
    {
        return m_data[key];
    }
};

}// namespace espresso