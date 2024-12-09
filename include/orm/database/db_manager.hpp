#pragma once

#include "orm/database/presets/sqlite_instance.hpp"

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
        return *m_db;
    }

    template<typename... Types>
    void execute_query(std::string_view query, std::function<void(rfl::Tuple<Types...>&)>&& callback)
    {
        rfl::Tuple<Types...> result;
        m_db->execute_query(query, [&result](std::vector<std::any>& vals) {
            [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                ((std::get<Is>(result) = std::any_cast<Types>(vals[Is])), ...);
            }(std::make_index_sequence<sizeof...(Types)>{});
        });
        callback(result);
    }

    template<typename Callable>
        requires(!is_specialization_of_v<Callable, std::function>)
    void execute_query(std::string_view query, Callable&& callback)
    {
        execute_query(query, std::function(std::forward<Callable>(callback)));
    }
};

}// namespace espresso::orm