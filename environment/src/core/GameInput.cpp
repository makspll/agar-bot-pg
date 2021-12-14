#include "core/GameInput.h"
#include <iostream>
#include <cstring>

int GameInput::serialize(char * buffer){
    return mouse_pos.serialize(buffer);
}

int GameInput::deserialize(char * buffer){
    return mouse_pos.deserialize(buffer);
}