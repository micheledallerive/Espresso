#include "http/response.hpp"
#include "http/server.hpp"
#include "routing/path.hpp"
#include "routing/router.hpp"
#include <iostream>

using namespace std;
using namespace espresso;
using namespace espresso::http;

Router hello_routes()
{
    Router r;
    r.route("/")
            .get([](const Request &request, Response &response) {
                response.send_file("/home/michele/index.html");
            });
    r.route("/{name}")
            .get([](const Request& request, Response& response) {
                const auto& name = request.url_params().at("name");
                response.write("Hello, " + name + "!");
                //                response.send_file("/home/michele/index.html");
            })
            .post([](const Request& request, Response& response) {
                response.write("Hello, POST!");
            });
    return r;
}

int main()
{
    Server server;
    server.router().route("/hello", hello_routes());
    server.listen(8080);
    return 0;
}
