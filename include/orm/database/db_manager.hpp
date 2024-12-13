#pragma once

#include "orm/database/types.hpp"
#include "orm/database/concepts.hpp"
#include "orm/database/presets/sqlite_instance.hpp"
#include "utils/tuple.hpp"

namespace espresso::orm {

static_assert(DBInstanceConcept<EspressoSettings::DB>, "The database type specified in EspressoSettings::DB does not satisfy the DBInstanceConcept");

/**
 * Singleton class to handle database
 */
class DBManager {
private:
    DBManager() = default;

    std::optional<DB> m_db{std::nullopt};

public:
    //using Compiler = DB::Compiler;
    static DBManager& get()
    {
        static DBManager instance;
        return instance;
    }

    template<typename... Args>
    void emplace(Args&&... args)
    {
        m_db.emplace(std::forward<Args>(args)...);
    }

    DB& db()
    {
        return m_db.value();
    }

    void execute_query(std::string_view query)
    {
        m_db->execute_query(query);
    }

    template<typename... Types>
    void execute_query(std::string_view query, std::function<void(const Tuple<Types...>&)>&& callback)
    {
        m_db->execute_query(query, std::function([&callback](std::vector<std::any>& vals) {
                                const auto t = vector_to_tuple<Types...>(vals);
                                callback(t);
                            }));
    }

    template<typename Callable>
        requires(!is_specialization_of_v<Callable, std::function>)
    void execute_query(std::string_view query, Callable&& callback)
    {
        execute_query(query, std::function(std::forward<Callable>(callback)));
    }
};

}// namespace espresso::orm