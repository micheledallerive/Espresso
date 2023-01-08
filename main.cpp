#include <iostream>
#include "lib/Server.h"

int main() {
    auto server = new Espresso::Server(8080);
    server->listen();
    return 0;
}
