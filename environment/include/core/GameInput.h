#ifndef GAME_INPUT_H
#define GAME_INPUT_H
#include "core/ISerializable.h"
#include "math/math.h"
#include "core/IPrintable.h"

struct GameInput : ISerializable, IPrintable{

    int serialize(char * buffer) const;
    int deserialize(char * buffer);

    bool operator==(const GameInput& o) const { 
        return mouse_pos == o.mouse_pos;
    }
    
    bool operator!=(const GameInput& o) const { 
        return !this->operator==(o);
    }

    std::ostream& print(std::ostream& s) const{
        s << "GameInput: " << mouse_pos << '\n';

   
        return s;
    }
    Vec2 mouse_pos;
};

#endif //GAME_INPUT_H