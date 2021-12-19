#include "core/GameState.h"
#include <assert.h> 


GameState::GameState(){
    collisionChecker = new BruteForce();
    for(auto const& x : players){
        for(int j = 0; j < x.second.parts.size();j++){
            collisionChecker->insert(*players[x.first].get_blob_state(j));
        }
    }
}

GameState::~GameState(){
    delete collisionChecker;
}

GameState::GameState(bool is_server):GameState(){
    this->is_server = is_server;
}

int GameState::to(char *buffer, int player){
    int bytes_total = 1;
    buffer[0] = players.size();
    buffer += 1;
    for(auto const& x : players){
        buffer[0] = x.first;
        buffer++;
        bytes_total++;
        int bytes_written = get_player_state(x.first)->serialize(buffer);
        buffer+=bytes_written;
        bytes_total += bytes_written;
    }


    buffer[0] = food_pellets.size();
    buffer+=1;
    bytes_total+=1;
    for(auto const& x : food_pellets){
        buffer[0] = x.first;
        buffer++;
        bytes_total++;
        int bytes_written = get_food_state(x.first)->serialize(buffer);
        buffer+=bytes_written;
        bytes_total += bytes_written;
    }

    return bytes_total;
}

int GameState::from(char * buffer, int ignore_pid){

    int bytes_total = 0;
    int player_count = buffer[0];
    bytes_total += 1;
    buffer += 1;
    // PLAYERS
    // see if players were removed
    bool players_removed = false;
    if(player_count < players.size()){
        players_removed = true;
        temp_ids.clear();
    }

    for(int i = 0; i < player_count; i++){
        int pid = buffer[0];
        buffer++;
        bytes_total++;
        bool new_player = !players.count(pid);

        if(new_player){
            add_player(pid);
        } else if (players_removed){
            temp_ids.insert(pid);
        }

        int bytes_read = 0;
        if(pid == ignore_pid && !new_player){
            bytes_read += nullPlayer.deserialize(buffer);
        } else {
            bytes_read += get_player_state(pid)->deserialize(buffer);
        }
        buffer += bytes_read;
        bytes_total += bytes_read;
    }
    
    // remove removed players
    if(players_removed){
        // delete ids not in new state
        // while safely iterating 
        for (auto it = players.cbegin(); it != players.cend();){
            if(!temp_ids.count(it->first))
                remove_player((it++)->first);
            else
                ++it;
        }
    }


    int pellets_count = buffer[0];
    buffer += 1;
    bytes_total += 1;

    bool pellets_removed = false;
    if(pellets_count < food_pellets.size()){
        pellets_removed = true;
        temp_ids.clear();
    }

    for(int i = 0; i < pellets_count; i++){
        int fid = buffer[0];
        buffer++;
        bytes_total++;
        bool new_food = !food_pellets.count(fid);

        if(new_food){
            add_food(fid);
        } else if (pellets_removed){
            temp_ids.insert(fid);
        }

        int bytes_read = get_food_state(fid)->deserialize(buffer);
        buffer += bytes_read;
        bytes_total += bytes_read;
    }

    // remove removed pellets
    if(pellets_removed){
        // delete ids not in new state
        // while safely iterating 
        for (auto it = food_pellets.cbegin(); it != food_pellets.cend();){
            if(!temp_ids.count(it->first))
                remove_food((it++)->first);
            else
                ++it;
        }
        temp_ids.clear();
    }

    return bytes_total;
}

void GameState::draw(sf::RenderWindow * rw) const{
    for(auto const& x : players){
        x.second.draw(rw);
    }

    for(auto const& x : food_pellets){
        x.second.draw(rw);
    }
}

void GameState::set_input(int pid, GameInput input){
    get_player_state(pid)->curr_input = input;
}


void GameState::update(float dt){
      // generate food


    // generate random events on the server only
    // p(tick) = p(second) / ticks
    // p(tick) = p(second) / (1/TICK_SECONDS)
    // p(tick) = p(second) * TICK_SECONDS
    if(is_server){
        if(food_pellets.size() < MAX_FOOD_PELLETS && dis(gen) < (FOOD_PROB_PER_SECOND * TICK_SECONDS)){
            // find free food id
            for(int i = food_pellets.size() + ID::get_first_idx(EntityType::FOOD_PELLET); ; i++){
                if(i >= ID::get_first_idx(EntityType::FOOD_PELLET) + MAX_FOOD_PELLETS) // wrap around
                    i = ID::get_first_idx(EntityType::FOOD_PELLET);

                if(!food_pellets.count(i)){
                    add_food(i);
                    get_food_state(i)->id.entity = i;
                    get_food_state(i)->pos = Vec2((dis(gen) - 0.5)*(ARENA_HALF_WIDTH_UNITS*2*CLIENT_UNIT_LENGTH),
                                            (dis(gen) - 0.5)*(ARENA_HALF_WIDTH_UNITS*2*CLIENT_UNIT_LENGTH));

                    break;
                }
            }
        }
    }
    
    for(auto const&x : players){
        auto curr_player = &players[x.first];
        if(curr_player->parts.size() > 0){
            Vec2 to_mouse = curr_player->curr_input.mouse_pos - curr_player->pos;
            to_mouse.normalize();
            to_mouse = to_mouse * (dt * INITIAL_SPEED_UNITS_PER_SECOND * CLIENT_UNIT_LENGTH);
            curr_player->pos = Vec2();
            for(int j = 0; j < curr_player->parts.size(); j++){
                curr_player->get_blob_state(j)->pos = curr_player->get_blob_state(j)->pos  + to_mouse;
                curr_player->get_blob_state(j)->pos.clamp((float)-ARENA_HALF_WIDTH_UNITS * CLIENT_UNIT_LENGTH,(float)ARENA_HALF_WIDTH_UNITS * CLIENT_UNIT_LENGTH);
                curr_player->pos = curr_player->pos + curr_player->get_blob_state(j)->pos;
            }
            curr_player->pos = curr_player->pos / curr_player->parts.size();
        }
    }

    // collision checks + resolutions

    // collision detection
    if(is_server){
        std::vector<Hit> hits = collisionChecker->hits();

        for(int i = 0; i < hits.size(); i++){
            // player player collions
            Hit& curr_hit = hits[i];
                
            // player ids (0-MAX_PLAYERS) are mapped to entity ids as well
            // player player collision
            EntityType e1 = curr_hit.entity1.to_entity_type();
            EntityType e2 = curr_hit.entity2.to_entity_type();
            
            int eOR = (int)e1 | (int)e2;

            // vs pellet
            if(eOR & (int)EntityType::FOOD_PELLET){
                // player vs pellet
                if(eOR & (int)EntityType::PLAYER_BLOB){
                    if(e1 != EntityType::FOOD_PELLET) // make sure e1 is the pellet
                        std::swap(curr_hit.entity1,curr_hit.entity2);
                    if(curr_hit.overlap > 0.5){
                        auto player = get_player_state(curr_hit.entity2.entity);
                        auto blob = player->get_blob_state(curr_hit.entity2.sub_entity);
                        auto food = get_food_state(curr_hit.entity1.entity);
                        auto mass = food->get_mass() + blob->get_mass();
                        blob->set_mass(mass);
                        remove_food(curr_hit.entity1.entity);
                    } 
                }
            
            } else if (eOR & (int)EntityType::PLAYER_BLOB){
                // player vs player 
                if(curr_hit.overlap > 1.1){
                    BlobState* biggerPart = &get_player_state(curr_hit.entity1.entity)->parts[curr_hit.entity1.sub_entity];
                    BlobState* eatenPart = &get_player_state(curr_hit.entity2.entity)->parts[curr_hit.entity2.sub_entity];
                    if(biggerPart->get_mass() < eatenPart->get_mass()){
                        std::swap(biggerPart,eatenPart);
                        std::swap(curr_hit.entity1,curr_hit.entity2);
                    }
                    biggerPart->set_mass(biggerPart->get_mass() + eatenPart->get_mass());
                    get_player_state(curr_hit.entity2.entity)->remove_part(eatenPart->id.sub_entity);
                    collisionChecker->remove(*eatenPart);
                }
            }
                    
        }
    }
}