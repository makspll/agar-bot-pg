#ifndef HIT_H
#define HIT_H
#include "math/math.h"
#include "core/EntityType.h"

struct ID{
    ID(){
        entity = -1;
        sub_entity = -1;
    }

    ID(int entity, int sub_entity)
    : entity(entity), sub_entity(sub_entity){}

    int entity = -1;
    int sub_entity = -1;

    inline EntityType to_entity_type(){
        if(entity < MAX_PLAYERS)
            return EntityType::PLAYER_BLOB;
        else if (entity < MAX_FOOD_PELLETS + MAX_PLAYERS)
            return EntityType::FOOD_PELLET;
        else if (entity > 0)
            return EntityType::VIRUS;
        return (EntityType)0;
    }

    static inline int get_first_idx(EntityType t){
        switch(t){
            case EntityType::PLAYER_BLOB:   
                return 0;
            case EntityType::FOOD_PELLET:
                return MAX_PLAYERS;
            case EntityType::VIRUS:
                return MAX_PLAYERS + MAX_FOOD_PELLETS;
        }

    };

    bool operator==(const ID& o) const { 
        return entity == o.entity && sub_entity == o.sub_entity; 
    }

};

struct Hit{
    Vec2 normal;
    Vec2 pos;
    float overlap = 0;
    ID entity1;
    ID entity2;
};


#endif //HIT_H