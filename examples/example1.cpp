#include "http/response.hpp"
#include "http/server.hpp"
#include "routing/path.hpp"
#include "routing/router.hpp"
#include <fstream>
#include <iostream>

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
            .get([](const Request& request, Response& response, Route::NextFunction next) {
                response.send_file("./index.html");
            });
    r.route("/{name}")
            .get([](const Request& request, Response& response, Route::NextFunction next) {
                const auto& name = request.url_params().at("name");
                response.write("Hello, " + name + "!");
                next();
            })
            .get([](const Request& request, Response& response, Route::NextFunction next) {
                const auto& name = request.url_params().at("name");
                response.write("Again, hello " + name + "!");
            })
            .post([](const Request& request, Response& response, Route::NextFunction next) {
                response.write("Hello, POST!");
            });
    return r;
}

int main()
{
    create_sample_html_file();
    Server server;
    server.router().route("/hello", hello_routes());
    server.listen(8080);
    return 0;
}
