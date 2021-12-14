#include "state/PlayerState.h"

int PlayerState::serialize(char * buffer){
    buffer[0] = pid;
    buffer[1] = view_scale;
    buffer[2] = part_count;
    int bytes = 3;
    buffer += bytes;
    bytes += pos.serialize(buffer) + 1;

    for(int i = 0; i < part_count ; i++){
        int written =  parts[i].serialize(buffer);
        buffer += written;
        bytes += written;
    }

    return bytes;
}

int PlayerState::deserialize(char * buffer){
    pid = buffer[0];
    view_scale = buffer[1];
    part_count = buffer[2];
    int bytes = 3;
    buffer += bytes;
    bytes += pos.deserialize(buffer) + 1;

    for(int i = 0; i < part_count; i++){
        int read = parts[i].deserialize(buffer);
        buffer+= read;
        bytes += read;
    }
    
    return bytes;
}

void PlayerState::draw(sf::RenderWindow * rw){
    for(int i = 0; i < part_count; i++){
        parts[i].draw(rw);
    }
}



