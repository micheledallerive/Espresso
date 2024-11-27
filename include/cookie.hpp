#pragma once

#include <sstream>
#include <string>
#include <string_view>

namespace espresso {

class Cookie {
private:
    std::string m_name;
    std::string m_value;

    std::string m_path{};
    std::string m_domain{};
    std::size_t max_age{0};

    bool m_secure{false};
    bool m_http_only{false};

public:
    Cookie(std::string_view name, std::string_view value);

    Cookie& set_path(std::string_view path);

    Cookie& set_domain(std::string_view domain);

    Cookie& set_max_age(std::size_t age);

    Cookie& set_secure() noexcept;
    Cookie& reset_secure() noexcept;

    Cookie& set_http_only() noexcept;
    Cookie& reset_http_only() noexcept;

    [[nodiscard]] std::string serialize() const noexcept;
};

}// namespace espresso