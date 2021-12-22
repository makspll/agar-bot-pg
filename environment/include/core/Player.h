#ifndef PLAYER_H
#define PLAYER_H
#include "core/GameInput.h"

struct Player {
    
    unsigned int address = 0;
    unsigned int port = 0; 
    float time_without_contact = 0.0l;
    unsigned int last_processed_sequence_number = 0;
};


#endif // PLAYER_H