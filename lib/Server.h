//
// Created by michele on 08.01.23.
//

#ifndef ESPRESSO_SERVER_H
#define ESPRESSO_SERVER_H

const int ESPRESSO_MAX_CONNECTIONS = 100;
const unsigned int ESPRESSO_DEFAULT_PORT = 8888;

namespace Espresso {

    class Server {
    private:
        unsigned short int _port;
        int _socket;
    public:
        explicit Server();

        ~Server();

        void listen(unsigned short int port = ESPRESSO_DEFAULT_PORT, int max_connections = ESPRESSO_MAX_CONNECTIONS);
    };

} // Espresso

#endif //ESPRESSO_SERVER_H
