#include "state/PlayerState.h"
#include "math/math.h"

int PlayerState::serialize(char * buffer) const{
    int bytes = 0;

    buffer[0] = pid;
    buffer[1] = view_scale;
    buffer[2] = parts.size();
    increment(buffer, bytes, 3);
    increment(buffer, bytes, pos.serialize(buffer));

    increment(buffer,bytes,curr_input.serialize(buffer));

    for(const auto&x : parts){
        buffer[0] = x.first;
        increment(buffer,bytes,1);
        increment(buffer,bytes, x.second.serialize(buffer));
    }

    return bytes;
}

int PlayerState::deserialize(char * buffer){
    int bytes = 0;
    pid = buffer[0];
    view_scale = buffer[1];
    int part_count = buffer[2];

    increment(buffer,bytes, 3);
    increment(buffer,bytes, pos.deserialize(buffer));
    increment(buffer,bytes,curr_input.deserialize(buffer));

    bool parts_removed = false;
    if(part_count < parts.size()){
        parts_removed = true;
        temp_ids.clear();
    }

    for(int i = 0; i < part_count; i++){
        int pid_r = buffer[0];
        increment(buffer,bytes,1);

        bool new_part = !parts.count(pid_r);

        if(new_part)
            add_part(pid_r);
        if(parts_removed){
            temp_ids.insert(pid_r);
        }

        increment(buffer,bytes,get_blob_state(pid_r)->deserialize(buffer));
    }

    // remove removed parts
    if(parts_removed){
        for (auto it = parts.cbegin(); it != parts.cend();){
            if(!temp_ids.count(it->first))
                remove_part((it++)->first);
            else
                ++it;
        }

        temp_ids.clear();
    }
    
    return bytes;
}

void PlayerState::draw(sf::RenderWindow * rw) const{
    for(auto const&x : parts){
        x.second.draw(rw);
    }
}



