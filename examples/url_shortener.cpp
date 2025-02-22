#include <iostream>

#include "http/server.hpp"

#include <https/server.hpp>
#include <middleware/presets/json_parser.hpp>

using namespace std;
using namespace espresso;
using namespace espresso::http;

map<string, string> shortened_urls;

std::string generate_shorter_url()
{
    static int counter = 0;
    return "short_url_" + to_string(counter++);
}

int main()
{
    https::Server server(https::Server::Settings{.recv_timeout = 100s},
        https::Server::SSLSettings{.cert_file = "/home/michele/key/cert.pem", .key_file = "/home/michele/key/key.pem"});

    server.middleware(JSONParser());

    server.router()
            .get("/test", [&](const auto& req, auto& res) {
                res.write("Hello world.");
                res.status(200);
            })
            .get("/url/{short}", [&](const Request& req, Response& res) {
                const auto short_url = req.url_params().at("short");
                std::cout << "Short url: " << short_url << " Present? " << std::boolalpha << (shortened_urls.find(short_url) != shortened_urls.end()) << std::endl;
                if (shortened_urls.find(short_url) == shortened_urls.end()) {
                    res.status(404);
                    res.write("URL not found");
                    return;
                }

                // redirect
                res.redirect(shortened_urls[short_url]);
            })
            .post("/urls", [&](const Request& req, Response& res) {
                const auto json_body = req.custom_data().get<nlohmann::json>("json");
                const auto url = json_body.at("url").get<string>();

                const std::string shorter = generate_shorter_url();

                shortened_urls[shorter] = url;
                nlohmann::json response_json = {
                        {"short_url", shorter}};
                res.write(response_json.dump());
            });

    server.listen(8080);
    return 0;
}
