#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H
#include "core/ISerializable.h"
#include "math/math.h"
#include "core/IDrawable.h"
#include "core/GameInput.h"
#include "core/IPrintable.h"
#include "state/BlobState.h"
#include "core/macros.h"

struct PlayerState : ISerializable, IDrawable, IPrintable{
    PlayerState(){}

    PlayerState(int id) : pid(id){
        parts[0].pos = pos;
        part_count = 1;

        for(int i = 0 ;i < MAX_BLOBS; i++){
            parts[i].id.entity = pid;
            parts[i].id.sub_entity = i;
        }
    }


    int serialize(char * buffer);
    int deserialize(char * buffer);
    void draw(sf::RenderWindow * rw);
    std::ostream& print(std::ostream& s) const{
        s << "Player with " << (int)part_count << " parts - \n \tPID:"<<pid << ", POS:" << pos << '\n'; 
        for (int i = 0; i < part_count; i++){
            s << '\t';
            parts[i].print(s);
        }
    }

    int pid = -1;
    int view_scale = 1;
    Vec2 pos;
    GameInput curr_input;

    char part_count;
    BlobState parts[MAX_BLOBS];


};

#endif //PLAYER_STATE_H