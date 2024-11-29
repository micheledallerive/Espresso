#pragma once

#include "middleware/base_middleware.hpp"
#include <filesystem>

namespace espresso {

class StaticFiles : public BaseMiddleware {
private:
    std::string m_path_prefix;
    std::filesystem::path m_root_path;

public:
    StaticFiles(std::string path_prefix, std::filesystem::path root_path)
        : m_path_prefix(std::move(path_prefix)), m_root_path(std::move(root_path))
    {}
    ~StaticFiles() override = default;

    http::Response operator()(http::Request& request, middleware::NextFunctionRef next) override
    {
        if (!request.path().starts_with(m_path_prefix)) {
            return next(request);
        }

        auto file_path = m_root_path / request.path().substr(m_path_prefix.size() + 1);// +1 to exclude the leading slash
        if (!std::filesystem::exists(file_path)
            || !std::filesystem::is_regular_file(file_path)) {
            return next(request);
        }

        // SECURITY?
//        {
//            // check if the lexically normal paths have the root directory as the prefix
//            auto normal_path = std::filesystem::canonical(file_path).string();
//            if (!normal_path.starts_with(std::filesystem::canonical(m_root_path).string())) {
//                return next(request);
//            }
//        }

        http::Response response;
        response.status(200);
        response.send_file(file_path.string());
        return response;
    }
};

}// namespace espresso