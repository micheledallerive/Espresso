//
// Created by michele on 25.02.23.
//

#include <gtest/gtest.h>
#include <lib/json/JSON.h>

using namespace Espresso::JSON;

const std::string huge_json = R"(
{
  "name": "John",
  "age": 30,
  "cars": [
    { "name":"Ford", "models":[ "Fiesta", "Focus", "Mustang" ] },
    { "name":"BMW", "models":[ "320", "X3", "X5" ] },
    { "name":"Fiat", "models":[ "500", "Panda" ] }
  ],
    "address": {
        "street": "Main Street",
        "number": 123,
        "city": "New York"
    },
    "children": [
        {
            "name": "John",
            "age": 12
        },
        {
            "name": "Jane",
            "age": 10
        }
    ],
    "spouse": {
        "name": "Jane",
        "age": 30
    },
    "pets": [
        {
            "name": "Dog",
            "age": 5
        },
        {
            "name": "Cat",
            "age": 3
        }
    ]
}
)";

TEST(JSON, ParseHugeJSON) {
  JSON *json = JSON::parse(huge_json);
  EXPECT_EQ(json->getType(), JSONType::JSONObject);
}