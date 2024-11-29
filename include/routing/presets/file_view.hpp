#pragma once

#include "routing/base_route.hpp"
#include <filesystem>
#include <fstream>

namespace espresso {

class FileView : public BaseRouteWithNext {
private:
    std::filesystem::path m_path;
    std::string m_cache;

    [[nodiscard]] std::optional<std::string> read_file() const
    {
        std::ifstream ifs(m_path, std::ios::binary);
        if (ifs) {
            return std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
        }
        return std::nullopt;
    }

public:
    explicit FileView(std::filesystem::path path, bool cache = false)
        : m_path(std::move(path))
    {
        if (cache) {
            assert(exists(m_path));
            m_cache.assign(read_file().value());
        }
    }

    void operator()(const http::Request& request, http::Response& response, routing::NextFunctionRef next) override
    {
        response.status(200);
        response.headers().insert("Content-Type", mime_type(m_path));
        if (!m_cache.empty()) {
            response.write(m_cache);
            return;
        }

        auto file_content = read_file();
        if (file_content.has_value()) {
            response.write(file_content.value());
        }
        else {
            response.status(404);
        }
    }
};

}// namespace espresso