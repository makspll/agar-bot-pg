#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "state/PlayerState.h"
#include "core/macros.h"
#include "core/IDrawable.h"
#include "core/IPrintable.h"
#include <assert.h>
#include <stdexcept>

struct GameState : IDrawable, IPrintable{


    GameState(){
        for(int i =0; i < MAX_PLAYERS;i++){
            players[i].pid = -1;
        }
    }
    //TODO COPY AND ASSIGNMENT CONSTRUCTORS

    int player_count = 0;
    PlayerState players[MAX_PLAYERS];

    int to(char * buffer, int player);
    int from(char * buffer, int ignore_pid = -9);
    void add_player(int pid);
    void remove_player(int pid);
    PlayerState* get_player_state(int pid);
    void set_input(int pid, GameInput input);

    void draw(sf::RenderWindow * rw);

    std::ostream& print(std::ostream& s) const{
        s << "GameState containing: " << player_count << " players -" << '\n';
        for(int i = 0; i < player_count; i++){
            s << '\t';
            players[i].print(s);
            s << '\n';
        }
    }
    
    private:

    int find_player(int pid,bool err_on_nonexist=false){
        for(int i = 0;i < player_count;i++){
            if (players[i].pid == pid){
                return i;
            }
        }

        if(err_on_nonexist){
            throw std::logic_error("Player with given pid does not exist.");
        } else {
            return -1;
        }
    }

    
};


#endif //GAMESTATE_H