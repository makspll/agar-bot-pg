#ifndef ACOLLIDABLE_H
#define ACOLLIDABLE_H
#include "core/Hit.h"
#include "core/ICollidable.h"

class ACollidable : public ICollidable{

    public:

    ACollidable(unsigned int entity_id, unsigned int sub_entity_id, float radius, Vec2 pos)
     : id(entity_id,sub_entity_id),radius(radius), pos(pos) {}

    ID& get_ID(){return id;};
    Vec2& get_pos(){return pos;};
    float& get_radius(){return radius;};

    Vec2 pos;
    ID id;
    float radius = 0;
};


#endif //ACOLLIDABLE_H