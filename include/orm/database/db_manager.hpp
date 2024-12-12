#pragma once

#include "orm/database/presets/sqlite_instance.hpp"
#include "utils/tuple.hpp"

namespace espresso::orm {

/**
 * Singleton class to handle database
 */
class DBManager {
private:
    DBManager() = default;

    std::unique_ptr<DBInstance> m_db{nullptr};

public:
    static DBManager& get()
    {
        static DBManager instance;
        return instance;
    }

    template<typename Backend, typename... Args>
    void emplace(Args&&... args)
    {
        m_db = std::make_unique<Backend>(std::forward<Args>(args)...);
    }

    DBInstance& db()
    {
        if (m_db == nullptr)
            throw std::runtime_error("No database instance");
        return *m_db;
    }

    void execute_query(std::string_view query)
    {
        m_db->execute_query(query);
    }

    template<typename... Types>
    void execute_query(std::string_view query, std::function<void(const Tuple<Types...>&)>&& callback)
    {
        m_db->execute_query(query, [&callback](std::vector<std::any>& vals) {
            const auto t = vector_to_tuple<Types...>(vals);
            callback(t);
        });
    }

    template<typename Callable>
        requires(!is_specialization_of_v<Callable, std::function>)
    void execute_query(std::string_view query, Callable&& callback)
    {
        execute_query(query, std::function(std::forward<Callable>(callback)));
    }
};

}// namespace espresso::orm