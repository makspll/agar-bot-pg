#ifndef COLLISION_CHECKER_H
#define COLLISION_CHECKER_H
#include <vector>
#include "core/ICollidable.h"
#include "core/Hit.h"

class ICollisionChecker{
    public:

    virtual void insert(ICollidable &c) = 0;
    virtual void rebuild() = 0;

    virtual std::vector<Hit> hits() = 0;
};


#endif //COLLISION_CHECKER_H
