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

    // collision checks + resolutions

    // check new players were added, and update collision detection
    int player_diff = last_player_count - state.player_count;

    if(player_diff > 0){
        // players quit
        last_player_count = state.player_count;
    } else if (player_diff < 0) {
        // players joined
        for(int i = last_player_count; i < state.player_count;i++){
            for(int j = 0;j < state.players[i].part_count; j++){
                collisionChecker->insert(state.players[i].parts[j]);
            }
        }
        last_player_count = state.player_count;
    }

    std::vector<Hit> hits = collisionChecker->hits();

    for(int i = 0; i < hits.size(); i++){
        // player player collions
        Hit& curr_hit = hits[i];

        // player ids (0-MAX_PLAYERS) are mapped to entity ids as well
        if(curr_hit.entity1.entity < MAX_PLAYERS || curr_hit.entity2.entity < MAX_PLAYERS){
            std::cout << curr_hit.overlap << '\n'; 
            if(curr_hit.overlap > 0.9){
                std::cout << "EATEN" << '\n';
            }
        }
        
    }

}