#ifndef SIMULATOR_H
#define SIMULATOR_H
#include "core/GameInput.h"
#include "core/GameState.h"
#include "core/ICollisionChecker.h"
#include "collisions/BruteForce.h"

class Simulator{
    public:

    Simulator(GameState& state){
        collisionChecker = new BruteForce();
        for(int i = 0 ; i < state.player_count; i++){
            for(int j = 0; j < state.players[i].part_count;j++){
                collisionChecker->insert(state.players[i].parts[j]);
            }
        }
        last_player_count = state.player_count;
    }


    void update(GameState& state, float dt);


    ICollisionChecker* collisionChecker;
    int last_player_count;

};


#endif //SIMULATOR_H