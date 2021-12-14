#include "core/GameState.h"
#include <assert.h> 

int GameState::to(char *buffer, int player){
    int bytes_total = 1;
    buffer[0] = (char)player_count;
    buffer += 1;
    for(int i = 0; i < player_count; i++){
        int bytes_written = players[i].serialize(buffer);
        buffer+=bytes_written;
        bytes_total += bytes_written;
    }

    return bytes_total;
}

int GameState::from(char * buffer, int ignore_pid){
    PlayerState nullPlayer;

    int bytes_total = 1;
    int prev_player_count = player_count;
    player_count = (int)buffer[0];
    buffer += 1;

    for(int i = 0; i < player_count; i++){
        int bytes_read;
        
        // if player count changed, data past the last player count 
        // is changed, so cannot ignore it
        if (i < prev_player_count && players[i].pid == ignore_pid) {
            // discard data, but still need to know how many bytes
            bytes_read = nullPlayer.deserialize(buffer);
        }
        else
            bytes_read = players[i].deserialize(buffer);

        buffer += bytes_read;
        bytes_total += bytes_read;
    }


    return bytes_total;
}

void GameState::add_player(int pid){
    players[player_count] = PlayerState();
    players[player_count].pid = pid;

    player_count++;
}

void GameState::remove_player(int pid){

    // find player state
    int remove_idx = find_player(pid,true);
    players[remove_idx].pid = -1;
    
    // shift players one down (overwrite deleted)
    for( ;remove_idx < player_count - 1 && remove_idx > -1; remove_idx++){
        players[remove_idx] = players[remove_idx+1];
    }

    player_count--;
}    

void GameState::draw(sf::RenderWindow * rw){
    for(int i = 0; i < player_count; i++){
        players[i].draw(rw);
    }
}

void GameState::set_input(int pid, GameInput input){
    players[find_player(pid,true)].curr_input = input;
}

PlayerState* GameState::get_player_state(int pid){
    return players + find_player(pid,true);
}

