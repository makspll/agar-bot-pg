#ifndef SIMULATOR_H
#define SIMULATOR_H
#include "core/GameInput.h"
#include "core/GameState.h"

class Simulator{
    public:
    void update(GameState& state, float dt);

};


#endif //SIMULATOR_H