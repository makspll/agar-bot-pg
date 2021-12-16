#ifndef BLOB_STATE_H
#define BLOB_STATE_H
#include "math/math.h"
#include "core/IDrawable.h"
#include "core/IPrintable.h"
#include "core/ACollidable.h"
#include "core/ICollidable.h"
#include "core/macros.h"

struct BlobState : ISerializable, IDrawable, IPrintable, ACollidable{

    BlobState() : ACollidable(-1, -1,0,Vec2(ARENA_HALF_WIDTH_UNITS*CLIENT_UNIT_LENGTH * 2,ARENA_HALF_WIDTH_UNITS*CLIENT_UNIT_LENGTH * 2)) {
        set_radius_from_mass();
    }

 


    int serialize(char * buffer);
    int deserialize(char * buffer);
    void draw(sf::RenderWindow * rw);

    bool collides(ICollidable& o, Hit& h);


    std::ostream& print(std::ostream& s) const{
        s << "Blob ("<< id.sub_entity << ") - MASS:"<< mass << ", POS:" << pos << '\n'; 
    }

    void set_mass(int mass){
        this->mass = mass;
        set_radius_from_mass();
    }

    private:
    unsigned int mass = INITIAL_MASS;

    float set_radius_from_mass(){
        radius = pow(mass* 0.978394833 + 1.405197198 ,0.485133) * 0.228728 *  CLIENT_UNIT_LENGTH;
        return radius;
    }
    
};

#endif // BLOB_STATE_H