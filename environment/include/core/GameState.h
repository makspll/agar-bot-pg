#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "state/PlayerState.h"
#include "state/FoodPelletState.h"
#include "collisions/BruteForce.h"
#include "core/macros.h"
#include "core/IDrawable.h"
#include "core/IPrintable.h"
#include <assert.h>
#include <stdexcept>
#include <map>
#include <set>
#include <random>

struct GameState : IDrawable, IPrintable{


    GameState();
    ~GameState();
    GameState(bool is_server);

    //TODO COPY AND ASSIGNMENT CONSTRUCTORS

    std::map<int,PlayerState> players;
    std::map<int,FoodPelletState> food_pellets;

    int to(char * buffer, int player);
    int from(char * buffer, int ignore_pid = -9);
    void update(float dt);

    inline void add_player(int pid){
        pid += ID::get_first_idx(EntityType::PLAYER_BLOB);
        if(players.count(pid))
            throw std::invalid_argument("Player already exists.");

        players[pid] = PlayerState(pid);
        for(int i = 0 ; i < players.at(pid).parts.size();i++){
            collisionChecker->insert(*players.at(pid).get_blob_state(i));
        }
    };

    inline void remove_player(int pid){
        pid += ID::get_first_idx(EntityType::PLAYER_BLOB);
        if(!players.count(pid))
            throw std::invalid_argument("Player doesn't exist.");
        for(int i = 0 ; i < players.at(pid).parts.size();i++){
            collisionChecker->remove(*players.at(pid).get_blob_state(i));
        }
        players.erase(pid);
    };

    inline void add_food(int fid){
        if(food_pellets.size() + 1 > MAX_FOOD_PELLETS)
            throw std::invalid_argument("Maximum food pellets count already exists. (" + std::to_string(food_pellets.size()) + ")");


        food_pellets[fid] = FoodPelletState(food_pellets.size());    
        collisionChecker->insert(food_pellets.at(fid));
    };

    inline void remove_food(int fid){
        if(!food_pellets.count(fid))
            throw std::invalid_argument("Food doesn't exist.");

        collisionChecker->remove(food_pellets.at(fid));
        food_pellets.erase(fid);
    };

    inline PlayerState* get_player_state(int pid){
        return &players.at(pid);
    }   
    inline FoodPelletState* get_food_state(int fid){
        return &food_pellets.at(fid);
    }

    void set_input(int pid, GameInput input);

    void draw(sf::RenderWindow * rw) const;

    std::ostream& print(std::ostream& s) const{
        s << "GameState containing: " << players.size() << " players -" << '\n';

        for (auto const& x : players)
        {
            s << '\t';
            x.second.print(std::cout);
            s << '\n';
        }

        s << "Food -\n";

        for (auto const& x : food_pellets)
        {
            s << '\t';
            x.second.print(std::cout);
            s << '\n';
        }

        return s;
    }
    
    private:

    std::set<int> temp_ids;
    PlayerState nullPlayer;

    std::random_device rd;
    std::mt19937 gen = std::mt19937(rd());
    std::uniform_real_distribution<float> dis = std::uniform_real_distribution<float>(0,1);

    ICollisionChecker* collisionChecker;
    bool is_server = false;
};


#endif //GAMESTATE_H