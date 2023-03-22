//
// Created by michele on 25.02.23.
//

#include <gtest/gtest.h>
#include "lib/json/JSONEntity.h"

using namespace Espresso::JSON;

const std::string huge_json = R"(
[
    {
        "_id": "63fbb7404fe65d593c7df3bc",
        "index": 0,
        "guid": "19d38101-45a0-4cd5-b797-3e9844188bdb",
        "isActive": false,
        "balance": "$2,894.49",
        "picture": "http://placehold.it/32x32",
        "age": 35,
        "eyeColor": "blue",
        "name": "Johnnie Becker",
        "gender": "female",
        "company": "MATRIXITY",
        "email": "johnniebecker@matrixity.com",
        "phone": "+1 (972) 469-2420",
        "address": "814 McKibben Street, Graniteville, Florida, 3006",
        "about": "Sit mollit est pariatur amet adipisicing aliqua sunt occaecat. Amet dolore ut nulla aliquip ea eu laboris sint mollit. Do laboris dolor velit officia non cupidatat. Veniam fugiat ipsum laborum et ex ex. Est deserunt cupidatat eiusmod velit dolor duis nisi qui mollit sunt.\r\n",
        "registered": "2021-07-09T08:19:31 -02:00",
        "latitude": -34.210794,
        "longitude": 19.889162,
        "tags": [
            "sit",
            "sint",
            "aliquip",
            "ex",
            "excepteur",
            "incididunt",
            "fugiat"
        ],
        "friends": [
            {
                "id": 0,
                "name": "Sheppard Alvarado"
            },
            {
                "id": 1,
                "name": "Moore Solis"
            },
            {
                "id": 2,
                "name": "Clark Hill"
            }
        ],
        "greeting": "Hello, Johnnie Becker! You have 2 unread messages.",
        "favoriteFruit": "strawberry"
    },
    {
        "_id": "63fbb74090d2c21159df272d",
        "index": 1,
        "guid": "b222a173-196d-4779-95e7-fe1ee2ff81d8",
        "isActive": false,
        "balance": "$2,779.53",
        "picture": "http://placehold.it/32x32",
        "age": 32,
        "eyeColor": "green",
        "name": "Alvarado Schultz",
        "gender": "male",
        "company": "OATFARM",
        "email": "alvaradoschultz@oatfarm.com",
        "phone": "+1 (825) 402-3847",
        "address": "265 Howard Avenue, Jacumba, Ohio, 193",
        "about": "Exercitation velit ad elit exercitation non fugiat est cupidatat. Aliquip quis pariatur velit adipisicing do dolor duis in. Sint occaecat enim ut ipsum aliquip reprehenderit eu dolor do ut. Labore cupidatat in elit labore ut enim excepteur adipisicing cillum cupidatat tempor. Occaecat est nisi occaecat ut aute culpa labore aliqua id laboris sint.\r\n",
        "registered": "2014-05-11T12:23:40 -02:00",
        "latitude": -52.295569,
        "longitude": -92.86563,
        "tags": [
            "tempor",
            "deserunt",
            "adipisicing",
            "non",
            "Lorem",
            "anim",
            "eiusmod"
        ],
        "friends": [
            {
                "id": 0,
                "name": "Maude Cantu"
            },
            {
                "id": 1,
                "name": "Lindsay Ayala"
            },
            {
                "id": 2,
                "name": "Patrick Underwood"
            }
        ],
        "greeting": "Hello, Alvarado Schultz! You have 7 unread messages.",
        "favoriteFruit": "banana"
    },
    {
        "_id": "63fbb7406e23fae43e38a80a",
        "index": 2,
        "guid": "71bc3a26-0623-4fa7-9eab-06d3bb955230",
        "isActive": true,
        "balance": "$2,939.87",
        "picture": "http://placehold.it/32x32",
        "age": 32,
        "eyeColor": "blue",
        "name": "Brianna Valenzuela",
        "gender": "female",
        "company": "ZAGGLES",
        "email": "briannavalenzuela@zaggles.com",
        "phone": "+1 (907) 565-2008",
        "address": "271 Lafayette Walk, Holtville, North Carolina, 631",
        "about": "Amet duis dolor irure sunt. Labore excepteur aute ad cupidatat in. Ad eiusmod deserunt quis do nostrud elit esse.\r\n",
        "registered": "2017-12-29T10:32:24 -01:00",
        "latitude": -30.571596,
        "longitude": 39.80461,
        "tags": [
            "ex",
            "aliquip",
            "minim",
            "cillum",
            "do",
            "nostrud",
            "eu"
        ],
        "friends": [
            {
                "id": 0,
                "name": "Wiggins Hendricks"
            },
            {
                "id": 1,
                "name": "Slater Hicks"
            },
            {
                "id": 2,
                "name": "Evans Meyers"
            }
        ],
        "greeting": "Hello, Brianna Valenzuela! You have 7 unread messages.",
        "favoriteFruit": "strawberry"
    },
    {
        "_id": "63fbb740238334b84ebb84f7",
        "index": 3,
        "guid": "5bb8fb6c-997e-4359-8b1c-6f1cd9839ea9",
        "isActive": false,
        "balance": "$2,597.07",
        "picture": "http://placehold.it/32x32",
        "age": 35,
        "eyeColor": "blue",
        "name": "Tyler Tyson",
        "gender": "male",
        "company": "KAGGLE",
        "email": "tylertyson@kaggle.com",
        "phone": "+1 (849) 442-2597",
        "address": "716 Grove Place, Sedley, Marshall Islands, 9158",
        "about": "Elit occaecat est aute eu do reprehenderit. Officia ullamco ipsum duis magna culpa nostrud cillum officia adipisicing ex eiusmod culpa. Elit fugiat ullamco commodo id dolore sunt laboris quis nostrud qui mollit irure tempor aliquip. Laboris qui nulla et enim officia do eu ullamco qui consequat enim velit aliqua. Duis minim velit reprehenderit aliquip ut aliqua reprehenderit excepteur Lorem.\r\n",
        "registered": "2021-01-24T05:40:25 -01:00",
        "latitude": 53.546604,
        "longitude": 22.125414,
        "tags": [
            "nostrud",
            "occaecat",
            "ex",
            "labore",
            "esse",
            "qui",
            "anim"
        ],
        "friends": [
            {
                "id": 0,
                "name": "Hansen Gibson"
            },
            {
                "id": 1,
                "name": "Charles Hanson"
            },
            {
                "id": 2,
                "name": "Katy Oneal"
            }
        ],
        "greeting": "Hello, Tyler Tyson! You have 3 unread messages.",
        "favoriteFruit": "apple"
    },
    {
        "_id": "63fbb740657a23020f6ccd6e",
        "index": 4,
        "guid": "bf455760-7e50-44f3-94a4-badf3e526040",
        "isActive": true,
        "balance": "$3,794.55",
        "picture": "http://placehold.it/32x32",
        "age": 27,
        "eyeColor": "green",
        "name": "Johns Rivas",
        "gender": "male",
        "company": "URBANSHEE",
        "email": "johnsrivas@urbanshee.com",
        "phone": "+1 (949) 588-2248",
        "address": "555 Madeline Court, Ballico, Wisconsin, 3390",
        "about": "Sunt qui pariatur tempor excepteur. Est laborum proident consectetur in excepteur qui aliquip. Laboris in occaecat nostrud ad voluptate labore excepteur excepteur. Nulla culpa veniam quis deserunt ipsum aute laborum. Id sunt fugiat ea nulla sint esse ea non quis ut eu magna.\r\n",
        "registered": "2021-03-01T04:49:18 -01:00",
        "latitude": 63.477174,
        "longitude": -69.211363,
        "tags": [
            "sint",
            "sint",
            "proident",
            "qui",
            "labore",
            "et",
            "consectetur"
        ],
        "friends": [
            {
                "id": 0,
                "name": "Darcy Cobb"
            },
            {
                "id": 1,
                "name": "Mcclure Black"
            },
            {
                "id": 2,
                "name": "Theresa Kirkland"
            }
        ],
        "greeting": "Hello, Johns Rivas! You have 6 unread messages.",
        "favoriteFruit": "apple"
    },
    {
        "_id": "63fbb7408768cdb40755c6a5",
        "index": 5,
        "guid": "ccd8c7bc-e3ba-46ea-8a4c-a010473ed67e",
        "isActive": false,
        "balance": "$2,616.42",
        "picture": "http://placehold.it/32x32",
        "age": 29,
        "eyeColor": "brown",
        "name": "Victoria Holland",
        "gender": "female",
        "company": "AUSTEX",
        "email": "victoriaholland@austex.com",
        "phone": "+1 (999) 459-3332",
        "address": "377 Clarendon Road, Neibert, Minnesota, 7861",
        "about": "In ex qui magna et adipisicing nisi eiusmod ut labore ad ad. Elit commodo pariatur eiusmod id nostrud aute sit voluptate mollit nisi sint sint occaecat officia. Anim fugiat aliquip est consequat eiusmod reprehenderit ullamco ea in ut laboris non aliquip.\r\n",
        "registered": "2022-11-30T11:03:48 -01:00",
        "latitude": 61.130894,
        "longitude": -163.190999,
        "tags": [
            "est",
            "tempor",
            "occaecat",
            "commodo",
            "est",
            "id",
            "ullamco"
        ],
        "friends": [
            {
                "id": 0,
                "name": "Earline Shelton"
            },
            {
                "id": 1,
                "name": "Garner Petersen"
            },
            {
                "id": 2,
                "name": "Duran Lowery"
            }
        ],
        "greeting": "Hello, Victoria Holland! You have 8 unread messages.",
        "favoriteFruit": "apple"
    },
    {
        "_id": "63fbb740e882c45957576e59",
        "index": 6,
        "guid": "988054e7-8efb-4efd-8962-aeeee7faf7ec",
        "isActive": true,
        "balance": "$2,386.73",
        "picture": "http://placehold.it/32x32",
        "age": 24,
        "eyeColor": "blue",
        "name": "Heather Crawford",
        "gender": "female",
        "company": "PULZE",
        "email": "heathercrawford@pulze.com",
        "phone": "+1 (909) 502-3034",
        "address": "994 Vermont Court, Vallonia, Puerto Rico, 7644",
        "about": "Dolore laboris ea ullamco consequat ea consectetur irure eu nulla do quis ullamco. Incididunt nulla culpa cillum elit. Nostrud amet dolor adipisicing do id irure eiusmod. In consectetur ex consequat labore fugiat ad dolor ut deserunt mollit non ut anim in. Lorem elit sit laborum esse pariatur incididunt deserunt eu cupidatat labore duis dolor consectetur exercitation. Eu et do culpa dolor eu esse nostrud aliquip ad.\r\n",
        "registered": "2020-03-14T02:23:42 -01:00",
        "latitude": 38.562036,
        "longitude": 78.221355,
        "tags": [
            "dolor",
            "et",
            "duis",
            "nostrud",
            "sunt",
            "ex",
            "tempor"
        ],
        "friends": [
            {
                "id": 0,
                "name": "Camille Spears"
            },
            {
                "id": 1,
                "name": "Coleen Whitehead"
            },
            {
                "id": 2,
                "name": "Dale Garrison"
            }
        ],
        "greeting": "Hello, Heather Crawford! You have 4 unread messages.",
        "favoriteFruit": "strawberry"
    },
    {
        "_id": "63fbb7401a1ba103dd15ba86",
        "index": 7,
        "guid": "aaf76a65-e38b-47ad-9ab5-8b460f7674bc",
        "isActive": false,
        "balance": "$3,911.54",
        "picture": "http://placehold.it/32x32",
        "age": 27,
        "eyeColor": "brown",
        "name": "Joan Cantrell",
        "gender": "female",
        "company": "ACUMENTOR",
        "email": "joancantrell@acumentor.com",
        "phone": "+1 (982) 490-3428",
        "address": "944 Gaylord Drive, Thomasville, Missouri, 3099",
        "about": "Magna reprehenderit eiusmod veniam pariatur est reprehenderit ex do quis do cupidatat. Fugiat voluptate nulla excepteur proident ipsum quis Lorem elit reprehenderit irure Lorem aliquip. Nisi ut esse do pariatur. Veniam ea aliquip deserunt deserunt ullamco culpa occaecat anim ad. Sint voluptate ad reprehenderit nostrud dolore minim consectetur voluptate occaecat id occaecat do.\r\n",
        "registered": "2018-05-16T02:59:07 -02:00",
        "latitude": -61.150753,
        "longitude": -121.948094,
        "tags": [
            "dolore",
            "ut",
            "aliqua",
            "sit",
            "ipsum",
            "laboris",
            "sunt"
        ],
        "friends": [
            {
                "id": 0,
                "name": "Gonzales Castillo"
            },
            {
                "id": 1,
                "name": "Padilla Moore"
            },
            {
                "id": 2,
                "name": "York Kirk"
            }
        ],
        "greeting": "Hello, Joan Cantrell! You have 5 unread messages.",
        "favoriteFruit": "banana"
    },
    {
        "_id": "63fbb740bab4cd7c5a3ba2f1",
        "index": 8,
        "guid": "5babb6a4-db3a-44fa-9e3a-b15dc2c97bee",
        "isActive": false,
        "balance": "$2,113.19",
        "picture": "http://placehold.it/32x32",
        "age": 36,
        "eyeColor": "green",
        "name": "Jimenez Velazquez",
        "gender": "male",
        "company": "TALAE",
        "email": "jimenezvelazquez@talae.com",
        "phone": "+1 (989) 490-3128",
        "address": "713 Amity Street, Camptown, Arizona, 2608",
        "about": "Sint exercitation sit et nulla nisi consectetur officia laboris enim dolor. Nulla aliqua aliqua ad est. Dolore sunt anim laborum id veniam proident tempor veniam id enim commodo exercitation velit. Esse eu exercitation cupidatat consectetur et nulla aliquip laborum minim enim ut exercitation. Sit aliqua nostrud laborum culpa consectetur.\r\n",
        "registered": "2020-04-10T04:45:37 -02:00",
        "latitude": 55.577737,
        "longitude": -69.744294,
        "tags": [
            "veniam",
            "in",
            "nisi",
            "do",
            "adipisicing",
            "nostrud",
            "sint"
        ],
        "friends": [
            {
                "id": 0,
                "name": "Aline Fowler"
            },
            {
                "id": 1,
                "name": "Belinda Wiggins"
            },
            {
                "id": 2,
                "name": "Brooke Mcintosh"
            }
        ],
        "greeting": "Hello, Jimenez Velazquez! You have 2 unread messages.",
        "favoriteFruit": "banana"
    },
    {
        "_id": "63fbb740c422564eee58458a",
        "index": 9,
        "guid": "18ebe653-82f7-4ba3-9907-3156e2590cb9",
        "isActive": true,
        "balance": "$1,781.88",
        "picture": "http://placehold.it/32x32",
        "age": 33,
        "eyeColor": "green",
        "name": "Bryant Sharp",
        "gender": "male",
        "company": "IDETICA",
        "email": "bryantsharp@idetica.com",
        "phone": "+1 (889) 520-3383",
        "address": "225 Wyona Street, Fruitdale, Oklahoma, 2505",
        "about": "Tempor commodo velit aliquip Lorem laborum fugiat nostrud ipsum non adipisicing ut eiusmod Lorem consequat. Non deserunt et culpa aute sunt esse veniam. Qui ipsum tempor minim minim enim pariatur. Tempor mollit ut nulla deserunt officia sint ut. Enim consequat pariatur id enim eiusmod officia ut duis exercitation cupidatat pariatur voluptate. Adipisicing mollit officia aute eiusmod minim voluptate ad veniam adipisicing velit eu sunt aliqua. Est eiusmod laborum incididunt sunt nulla sint exercitation est dolore nostrud Lorem reprehenderit enim veniam.\r\n",
        "registered": "2017-06-25T11:30:38 -02:00",
        "latitude": -5.043479,
        "longitude": 116.879588,
        "tags": [
            "cillum",
            "ipsum",
            "dolore",
            "dolor",
            "in",
            "exercitation",
            "velit"
        ],
        "friends": [
            {
                "id": 0,
                "name": "Tamera Clements"
            },
            {
                "id": 1,
                "name": "Karin Welch"
            },
            {
                "id": 2,
                "name": "Vicki Perkins"
            }
        ],
        "greeting": "Hello, Bryant Sharp! You have 7 unread messages.",
        "favoriteFruit": "banana"
    },
    {
        "_id": "63fbb7409d07a552455d49aa",
        "index": 10,
        "guid": "7a762512-76b7-4fdf-a308-3f8d4f690aa1",
        "isActive": true,
        "balance": "$2,774.63",
        "picture": "http://placehold.it/32x32",
        "age": 30,
        "eyeColor": "green",
        "name": "Dunlap Chandler",
        "gender": "male",
        "company": "WEBIOTIC",
        "email": "dunlapchandler@webiotic.com",
        "phone": "+1 (969) 501-3297",
        "address": "823 Madison Place, Cedarville, Iowa, 2223",
        "about": "Dolor et labore fugiat fugiat pariatur exercitation fugiat reprehenderit irure officia Lorem id minim magna. Pariatur nisi deserunt exercitation nulla officia quis veniam tempor sint commodo. Proident eu est laborum ea est non adipisicing veniam sit exercitation qui sit pariatur. Ipsum Lorem labore eiusmod ex aute esse. Culpa aliqua ipsum eu incididunt dolore aliqua id nulla sunt adipisicing est. Reprehenderit duis aliquip do quis veniam sit elit cillum minim sit nulla irure commodo.\r\n",
        "registered": "2018-07-08T11:24:19 -02:00",
        "latitude": -29.521637,
        "longitude": -23.156379,
        "tags": [
            "proident",
            "laborum",
            "sit",
            "sint",
            "in",
            "aliquip",
            "irure"
        ],
        "friends": [
            {
                "id": 0,
                "name": "Decker Stein"
            },
            {
                "id": 1,
                "name": "Annie Salazar"
            },
            {
                "id": 2,
                "name": "Berg Matthews"
            }
        ],
        "greeting": "Hello, Dunlap Chandler! You have 6 unread messages.",
        "favoriteFruit": "strawberry"
    },
    {
        "_id": "63fbb740c8f296953a3fe5cd",
        "index": 11,
        "guid": "35092108-7e29-4182-b245-a050e7479967",
        "isActive": false,
        "balance": "$3,059.54",
        "picture": "http://placehold.it/32x32",
        "age": 29,
        "eyeColor": "blue",
        "name": "Levy Barron",
        "gender": "male",
        "company": "EMPIRICA",
        "email": "levybarron@empirica.com",
        "phone": "+1 (883) 574-3202",
        "address": "118 Vanderveer Place, Concho, Montana, 2550",
        "about": "Exercitation amet fugiat laboris nulla magna. Minim cillum voluptate excepteur consectetur reprehenderit quis nulla aliquip nulla qui. Officia est aliqua sunt incididunt cillum proident mollit adipisicing labore irure cupidatat officia sint.\r\n",
        "registered": "2023-01-10T12:06:24 -01:00",
        "latitude": 3.135623,
        "longitude": -133.824063,
        "tags": [
            "cupidatat",
            "voluptate",
            "nostrud",
            "et",
            "aute",
            "labore",
            "exercitation"
        ],
        "friends": [
            {
                "id": 0,
                "name": "Ina Sanders"
            },
            {
                "id": 1,
                "name": "Gretchen Deleon"
            },
            {
                "id": 2,
                "name": "Selma Woodward"
            }
        ],
        "greeting": "Hello, Levy Barron! You have 2 unread messages.",
        "favoriteFruit": "apple"
    },
    {
        "_id": "63fbb7407b28bdbf7f941557",
        "index": 12,
        "guid": "0ba8f3bd-099e-492b-bfdd-c612176ef551",
        "isActive": true,
        "balance": "$2,551.86",
        "picture": "http://placehold.it/32x32",
        "age": 27,
        "eyeColor": "blue",
        "name": "Daniels Todd",
        "gender": "male",
        "company": "ECLIPTO",
        "email": "danielstodd@eclipto.com",
        "phone": "+1 (870) 472-3262",
        "address": "916 Doscher Street, Darrtown, Vermont, 8052",
        "about": "Ullamco eiusmod proident aliquip ex eu nulla. Laboris pariatur elit eiusmod sunt quis irure ullamco elit. Qui consequat consequat dolore ipsum elit aliquip fugiat cillum cillum qui.\r\n",
        "registered": "2020-09-21T06:53:33 -02:00",
        "latitude": 28.256833,
        "longitude": -86.940981,
        "tags": [
            "reprehenderit",
            "laborum",
            "Lorem",
            "eiusmod",
            "dolor",
            "ipsum",
            "exercitation"
        ],
        "friends": [
            {
                "id": 0,
                "name": "Cecelia Mclean"
            },
            {
                "id": 1,
                "name": "Manning Sampson"
            },
            {
                "id": 2,
                "name": "Cecile Hartman"
            }
        ],
        "greeting": "Hello, Daniels Todd! You have 1 unread messages.",
        "favoriteFruit": "banana"
    }
]
)";

#include <chrono>

TEST(JSONEntity, ParseHugeJSON) {
  auto start = std::chrono::high_resolution_clock::now();
  std::shared_ptr<JSONEntity> json = JSONEntity::parse(huge_json);
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "Time taken by function: " << duration.count() << " microseconds"
            << std::endl;
  EXPECT_EQ(json->getType(), JSONType::ARRAY);
}