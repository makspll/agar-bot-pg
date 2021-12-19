#ifndef FOOD_PELLET_STATE_H
#define FOOD_PELLET_STATE_H
#include "core/ISerializable.h"
#include "core/AMassContaining.h"
#include "core/IPrintable.h"

struct FoodPelletState : ISerializable, IPrintable,AMassContaining {
    
    FoodPelletState();
    FoodPelletState(int fid);

    int serialize(char * buffer) const;
    int deserialize(char * buffer);

    std::ostream& print(std::ostream& s) const{
        s << "Food ("<< id.entity << ") - MASS:"<< mass << ", POS:" << pos << ", RAD:" << radius << '\n'; 
        return s;
    }

};

#endif // FOOD_PELLET_STATE_H