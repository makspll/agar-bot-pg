#include "state/FoodPelletState.h"

FoodPelletState::FoodPelletState(int fid): AMassContaining(){
    id.entity = fid;
    set_mass(1);
}


FoodPelletState::FoodPelletState(): AMassContaining(){
    set_mass(1);
}

int FoodPelletState::serialize(char * buffer) const{
    int bytes = 1;
    buffer[0] = id.entity;
    buffer++;

    bytes += pos.serialize(buffer);
    return bytes;
}

int FoodPelletState::deserialize(char * buffer){
    int bytes = 1;
    id.entity = buffer[0];
    buffer++;

    bytes += pos.deserialize(buffer);
    return bytes;
}