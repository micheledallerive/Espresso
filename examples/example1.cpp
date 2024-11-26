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
        auto name_cookie = request.headers().get_cookie("name");
        if (name_cookie) {
            std::cout << "I remember you, you are " << name_cookie.value() << std::endl;
        }
        const auto& name = request.url_params().at("name");
        response.add_cookie(Cookie("name", name));
        response.write("Hello, " + name + "!");
    });
    server.listen(8080);
    return 0;
}
