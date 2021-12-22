#ifndef SERVER_H
#define SERVER_H
#include "core/Player.h"
#include "core/macros.h"
#include "core/GameState.h"
#include "core/PacketBuffer.h"


class Server{
    public:
        Server(int port);
    
    private:

        void Spin();
    
        Player players[MAX_PLAYERS];
        GameState state = GameState(true);
        int socket_in_fd;
        PacketBuffer<GameInput> received_buffers[MAX_PLAYERS];
};

#endif //SERVER_H