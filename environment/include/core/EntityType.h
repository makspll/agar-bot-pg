#ifndef ENTITY_TYPE_H
#define ENTITY_TYPE_H

enum class EntityType{
    PLAYER_BLOB = 1 << 0, // 0001
    FOOD_PELLET = 1 << 1, // 0010
    VIRUS       = 1 << 2, // 0100
};

 


#endif // ENTITY_TYPE_H