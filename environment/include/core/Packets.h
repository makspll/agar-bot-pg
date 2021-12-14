#ifndef PACKETS_H
#define PACKETS_H

enum class ClientPacket : unsigned char 
{
    JOIN,
    LEAVE,
    INPUT,
    GAME_STATE,
};

enum class ServerPacket : unsigned char
{
    RESULT_FAIL,
    RESULT_SUCCESS,
    GAME_STATE,
};


#endif //PACKETS_H