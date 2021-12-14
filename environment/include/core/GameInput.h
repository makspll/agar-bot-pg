#ifndef GAME_INPUT_H
#define GAME_INPUT_H
#include "core/ISerializable.h"
#include "math/math.h"


struct GameInput : ISerializable{

    int serialize(char * buffer);
    int deserialize(char * buffer);

    Vec2 mouse_pos;
};

#endif //GAME_INPUT_H