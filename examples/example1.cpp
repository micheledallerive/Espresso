#include "http/response.hpp"
#include "http/server.hpp"
#include "routing/path.hpp"
#include "routing/router.hpp"
#include <iostream>

using namespace std;
using namespace espresso;
using namespace espresso::http;

int main()
{
    Server server;
    server.router().route("/hello/{name}/world").use(Method::GET, [](const Request& request, Response& response) {
        response.write("Hello, " + request.url_params()["name"] + "!");
    });
    server.listen(8080);
    return 0;
}
