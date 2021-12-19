#ifndef AMASS_CONTAINING_H
#define AMASS_CONTAINING_H
#include "core/macros.h"
#include <math.h>
#include "core/ACollidable.h"
#include "core/IDrawable.h"

class AMassContaining : public ACollidable, public IDrawable{

    public:
    AMassContaining();
    
    virtual void set_mass(unsigned int mass);
    virtual unsigned int get_mass(){
        return mass;
    };

    virtual void draw(sf::RenderWindow * rw) const;
    
    virtual bool collides(ICollidable& o, Hit& h);

    bool operator==(const AMassContaining& o) const { 
        return (mass == o.mass &&
        pos == o.pos &&
        radius == o.radius &&
        id == o.id);
    }
    bool operator!=(const AMassContaining& o) const { 
        return !this->operator==(o);
    }

    unsigned int mass = INITIAL_MASS;
    
    protected:

    float get_curr_radius();

    float set_radius_from_mass();


};


#endif // AMASS_CONTAINING_H