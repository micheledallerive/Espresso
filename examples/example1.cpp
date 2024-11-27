#include "http/response.hpp"
#include "http/server.hpp"
#include "middleware/base_middleware.hpp"
#include "routing/path.hpp"
#include "routing/router.hpp"
#include <fstream>
#include <iostream>

using namespace std;
using namespace espresso;
using namespace espresso::http;

class MyMiddleware : public BaseMiddleware {
public:
    http::Response operator()(http::Request& request, middleware::NextFunctionRef next) override
    {
        cout << "MyMiddleware 1" << endl;
        auto response = next(request);
        cout << "MyMiddleware 2" << endl;
        return response;
    }
};

class NotMyMiddleware : public BaseMiddleware {
public:
    http::Response operator()(http::Request& request, middleware::NextFunctionRef next) override
    {
        cout << "NotMyMiddleware 1" << endl;
        auto response = next(request);
        cout << "NotMyMiddleware 2" << endl;
        return response;
    }
};

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
            .get([](const Request& request, Response& response) {
                response.send_file("./index.html");
            });
    r.route("/{name}")
            .get([](const Request& request, Response& response) {
                const auto& name = request.url_params().at("name");
                response.write("Hello, " + name + "!\n");
                if (!request.query_params().empty()) {
                    response.write("Query parameters:\n");
                    for (const auto& [key, value] : request.query_params()) {
                        response.write(key + ": " + value + "\n");
                    }
                }
            })
            .get([](const auto& req, auto& res, auto next) {
                res.write("Hello, again!");
            })
            .post([](const Request& request, Response& response, Route::NextFunctionRef next) {
                response.write("Hello, POST!");
            });
    return r;
}

int main()
{
    create_sample_html_file();

    Server server;
    server.router().route("/hello", hello_routes());

    server.middleware(MyMiddleware());
    server.middleware(
            [](http::Request& request, middleware::NextFunctionRef next) {
                cout << "NotMyMiddleware 1" << endl;
                auto response = next(request);
                cout << "NotMyMiddleware 2" << endl;
                return response;
            });

    server.listen(8080);

    return 0;
}
