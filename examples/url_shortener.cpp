#include <iostream>

#include "http/server.hpp"

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
    Server server;

    server.middleware(JSONParser());

    server.router()
            .get("/url/{short}", [&](const Request& req, Response& res) {
                const auto short_url = req.url_params().at("short");
                if (shortened_urls.find(short_url) == shortened_urls.end()) {
                    res.status(404);
                    res.write("aURL not found");
                    return;
                }

                // redirect
                res.redirect(shortened_urls[short_url]);
            })
            .post("/urls", [&](const Request& req, Response& res) {
                const auto json_body = req.custom_data().get<nlohmann::json>("json");
                const auto url = json_body.at("url").get<string>();

                const std::string shorter = generate_shorter_url();

                //shortened_urls[shorter] = url;
                nlohmann::json response_json = {
                        {"short_url", shorter}};
                res.write(response_json.dump());
            });

    server.listen(8080);
    return 0;
}
