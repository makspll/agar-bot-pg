#ifndef SERVER_H
#define SERVER_H
#include "core/Player.h"
#include "core/macros.h"
#include "core/GameState.h"
#include "core/Simulator.h"

class Server{
    public:
        Server(int port);
    
    private:

        void Spin();
    
        Simulator simulator;
        Player players[MAX_PLAYERS];
        GameState state;
        int socket_in_fd;
};

#endif //SERVER_H