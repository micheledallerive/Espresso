#include <iostream>
#include "lib/Server.h"

int main() {
    auto server = new Espresso::Server();
    server->listen(8080);
    return 0;
}
