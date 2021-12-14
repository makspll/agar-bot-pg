#ifndef CLIENT_H
#define CLIENT_H
#include <SFML/Graphics.hpp>
#include <netinet/in.h>
#include "core/Simulator.h"
#include "core/GameState.h"

class Client{
    public:

    Client(int port, std::string ip);

    void spin();

    private:
    sf::RenderWindow* window; 
    struct sockaddr_in server_address;
    socklen_t server_add_len;
    int socket_in_fd;
    char own_pid = -1;

    Simulator simulator;
    GameState state;
};

#endif //CLIENT_H