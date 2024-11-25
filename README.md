# Espresso - Simple and fast web server

Espresso is a minimalistic and easy-to-use web server written in C++. It is
inspired by the popular JavaScript library, Express.js, and aims to bring
similar functionality to C++ developers.

I am not a proficient C++ developer and this project has the purpose of being a
learning experience for me. I am open to any suggestions and contributions.

## Features

- Routing: handle incoming requests using simple and flexible routing.
    - Supports GET, POST, PUT, DELETE, PATCH, OPTIONS, HEAD, and CONNECT
      requests.
    - Supports URL parameters.
- Middleware: use pre-defined or custom middleware to manipulate requests and
  responses.
- Request and response objects: access request data and manipulate the response.

## Getting Started

### Installation

```shell
$ git clone https://github.com/micheledallerive/Espresso.git
$ cd espresso
$ mkdir build
$ cd build
$ cmake ..
$ sudo make install
```

### Usage

Create a new file called `main.cpp` and add the following code:

```c++
#include <espresso.h>

int main() {
    Espresso::Server server;

    server.get("/", [](Espresso::Request *req, Espresso::Response *res) {
        res->send("Hello, World!");
    });

    server.listen(3000);

    return 0;
}
```

This will create a new Espresso server that listens on port 3000 and responds to
GET requests to the root route (/) with "Hello, World!".

The file can then be built using CMake or simply compiled with:

```shell
$ g++ main.cpp -o main -lespresso -std=c++2a
```

**Note**: the `-lespresso` flag is required to link the Espresso library.

## Notes

Please note, that usage and functionality may be limited and some features that
are present in Express may not be present in this project due to it being a
learning experience, so it is not intended for production use.