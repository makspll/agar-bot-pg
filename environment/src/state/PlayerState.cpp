#include "state/PlayerState.h"

int PlayerState::serialize(char * buffer) const{
    buffer[0] = pid;
    buffer[1] = view_scale;
    buffer[2] = parts.size();
    int bytes = 3;
    buffer += bytes;
    
    int written = pos.serialize(buffer);
    buffer += written;
    bytes += written;

    for(const auto&x : parts){
        buffer[0] = x.first;
        buffer++;
        bytes++;

        written = x.second.serialize(buffer);
        buffer += written;
        bytes += written;
    }

    return bytes;
}

int PlayerState::deserialize(char * buffer){
    pid = buffer[0];
    view_scale = buffer[1];
    int part_count = buffer[2];
    int bytes = 3;
    buffer += bytes;

    int read = pos.deserialize(buffer);
    buffer += read;
    bytes += read;
    
    bool parts_removed = false;
    if(part_count < parts.size()){
        parts_removed = true;
        temp_ids.clear();
    }

    for(int i = 0; i < part_count; i++){
        int pid_r = buffer[0];
        buffer++;
        bytes++;
        bool new_part = !parts.count(pid_r);

        if(new_part)
            add_part(pid_r);
        if(parts_removed){
            temp_ids.insert(pid_r);
        }

        read = get_blob_state(pid_r)->deserialize(buffer);
        buffer += read;
        bytes += read;
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



