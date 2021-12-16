#ifndef ICOLLIDABLE_H
#define ICOLLIDABLE_H
#include "core/Hit.h"

class ICollidable{

    public:

    virtual bool collides(ICollidable& o, Hit& h) = 0;
    virtual ID& get_ID() = 0;
    virtual Vec2& get_pos() = 0;
    virtual float& get_radius() = 0;

};


#endif //ICOLLIDABLE_H