#include "http/response.hpp"
#include "http/server.hpp"
#include "middleware/base_middleware.hpp"
#include "middleware/presets/json_parser.hpp"
#include "middleware/presets/static_files.hpp"
#include "routing/path.hpp"
#include "routing/router.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "routing/presets/file_view.hpp"

using namespace std;
using namespace espresso;
using namespace espresso::http;

void create_sample_html_file()
{
    ofstream file("./index.html");
    file << "<!DOCTYPE html>\n"
            "<html>\n"
            "<head>\n"
            "<title>Page Title</title>\n"
            "</head>\n"
            "<body>\n"
            "\n"
            "<h1>This is a Heading</h1>\n"
            "<p>This is a paragraph.</p>\n"
            "\n"
            "</body>\n"
            "</html>\n";
    file.close();
}

Router hello_routes()
{
    Router r;
    r.route("/")
            .get(FileView("./index.html", true));
    r.route("/{name}")
            .get([](const Request& request, Response& response, auto next) {
                const auto& name = request.url_params().at("name");
                response.write("Hello, " + name + "!\n");
                if (!request.query_params().empty()) {
                    response.write("Query parameters:\n");
                    for (const auto& [key, value] : request.query_params()) {
                        response.write(key + ": " + value + "\n");
                    }
                }
                next();
            })
            .get([](const http::Request& req, auto& res, auto next) {
                res.write("Hello, again!");
            })
            .post([](const Request& request, Response& response) {
                auto json = request.custom_data().get<nlohmann::json>("json");
                std::cout << json.dump(4) << std::endl;
                response.write("Hello, POST!");
            });
    return r;
}

int main()
{
    create_sample_html_file();

    Server server;
    server.router().route("/hello", hello_routes());

    server.middleware(JSONParser());
    server.middleware(StaticFiles("/static", "./CMakeFiles/"));

    server.listen(8080);

    return 0;
}
