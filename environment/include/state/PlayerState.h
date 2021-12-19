#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H
#include "core/ISerializable.h"
#include "math/math.h"
#include "core/IDrawable.h"
#include "core/GameInput.h"
#include "core/IPrintable.h"
#include "state/BlobState.h"
#include "core/macros.h"
#include <map>
#include <set>

struct PlayerState : ISerializable, IDrawable, IPrintable{
    PlayerState(){}

    PlayerState(int id) : pid(id){
        add_part(0);
        parts[0].pos = pos;
    }

    std::map<int,BlobState> parts;

    int serialize(char * buffer) const;
    int deserialize(char * buffer);

    void add_part(int part_id){
        std::cout << "NEW _PART " << part_id << '\n';
        parts[part_id] = BlobState(pid,part_id);
    }

    void remove_part(int part_id){
        std::cout << "REMOVED PART " << part_id << '\n';
        parts.erase(part_id);
    }

    BlobState* get_blob_state(int part_id) {
        return &parts.at(part_id);
    }

    void draw(sf::RenderWindow * rw) const;
    std::ostream& print(std::ostream& s) const{
        s << "Player with " << parts.size() << " parts - \n \tPID:"<<pid << ", POS:" << pos << '\n'; 
        for (auto const&x : parts){
            s << '\t';
            x.second.print(s);
        }
        return s;
    }
    bool operator==(const PlayerState& o) const { 
        if(!(pid == o.pid &&
                view_scale == o.view_scale &&
                pos == o.pos &&
                curr_input == o.curr_input &&
                parts.size() == o.parts.size()))
            return false;

        for (auto const&x : parts){
            if(x.second != o.parts.at(x.first)){
                return false;
            }
        }

        return true;
    }

    int pid = -1;
    int view_scale = 1;
    Vec2 pos;
    GameInput curr_input;
    std::set<int> temp_ids;
};

#endif //PLAYER_STATE_H