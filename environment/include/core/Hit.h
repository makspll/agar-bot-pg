#ifndef HIT_H
#define HIT_H
#include "math/math.h"


struct ID{
    ID(){
        entity = -1;
        sub_entity = -1;
    }

    ID(int entity, int sub_entity)
    : entity(entity), sub_entity(sub_entity){}

    int entity = -1;
    int sub_entity = -1;
};

struct Hit{
    Vec2 normal;
    Vec2 pos;
    float overlap = 0;
    ID entity1;
    ID entity2;
};


#endif //HIT_H