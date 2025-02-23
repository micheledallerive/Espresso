#include <https/server.hpp>

using namespace std;
using namespace espresso;

int main()
{
    https::Server server(https::Server::Settings{.recv_timeout = 1000s},
                         https::Server::SSLSettings{.cert_file = "/home/michele/key/cert.pem", .key_file = "/home/michele/key/key.pem"});

    server.router()
            .get("/test", [&](const auto& req, auto& res) {
                res.write("Hello world.");
                res.status(200);
            });

    server.listen(4433);
    return 0;
}
