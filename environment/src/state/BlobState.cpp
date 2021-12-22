#include "state/BlobState.h"
#include "core/macros.h"
#include <math.h>


BlobState::BlobState(): AMassContaining(){}
BlobState::BlobState(int pid, int sub_id):BlobState(){
    id.entity = pid;
    id.sub_entity = sub_id;
}

int BlobState::serialize(char * buffer) const{
    int bytes = 0;
    std::memcpy(buffer,&mass,sizeof(unsigned int));
    increment(buffer,bytes, sizeof(unsigned int));
    increment(buffer,bytes, pos.serialize(buffer));
    return bytes;
}

int BlobState::deserialize(char * buffer){
    int bytes = 0;
    unsigned int tmass;
    std::memcpy(&tmass,buffer,sizeof(unsigned int));
    increment(buffer,bytes,sizeof(unsigned int));
    set_mass(tmass);
    increment(buffer,bytes,pos.deserialize(buffer));

    return bytes;

}


