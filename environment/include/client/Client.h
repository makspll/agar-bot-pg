#ifndef CLIENT_H
#define CLIENT_H
#include <SFML/Graphics.hpp>
#include <netinet/in.h>
#include "core/GameState.h"
#include "core/PacketBuffer.h"

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
    
    unsigned int last_received_sequence_number = -1;
    unsigned int next_sequence_number = 0; 
    GameState state;
    PacketBuffer<GameInput> sent_buffer;
    PacketBuffer<GameState> received_buffer = PacketBuffer<GameState>(2);
    long double lag_ms;
};

#endif //CLIENT_H