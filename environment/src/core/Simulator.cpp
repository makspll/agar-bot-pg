#include "core/Simulator.h"
#include <math.h>
#include <algorithm>
#include "core/macros.h"

void Simulator::update(GameState& state, float dt){
    for(int i = 0; i < state.player_count; i++){

        Vec2 to_mouse = state.players[i].curr_input.mouse_pos - state.players[i].pos;
        to_mouse.normalize();
        to_mouse = to_mouse * (dt * INITIAL_SPEED_UNITS_PER_SECOND * CLIENT_UNIT_LENGTH);
        state.players[i].pos = Vec2();
        for(int j = 0; j < state.players[i].part_count; j++){
            state.players[i].parts[j].pos = state.players[i].parts[j].pos  + to_mouse;
            state.players[i].parts[j].pos.clamp((float)-ARENA_HALF_WIDTH_UNITS * CLIENT_UNIT_LENGTH,(float)ARENA_HALF_WIDTH_UNITS * CLIENT_UNIT_LENGTH);
            state.players[i].pos = state.players[i].pos + state.players[i].parts[j].pos;
        }
        state.players[i].pos = state.players[i].pos / state.players[i].part_count;
        
    }
}