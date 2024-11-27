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
    r.route("/{name}")
            .use(Method::GET, [](const Request& request, Response& response) {
                const auto& name = request.url_params().at("name");
                response.write("Hello, " + name + "!");
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
