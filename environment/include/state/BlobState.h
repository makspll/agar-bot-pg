#ifndef BLOB_STATE_H
#define BLOB_STATE_H
#include "math/math.h"
#include "core/AMassContaining.h"
#include "core/IPrintable.h"
#include "core/ACollidable.h"
#include "core/ICollidable.h"
#include "core/ISerializable.h"
#include "core/macros.h"

struct BlobState : ISerializable, IPrintable, AMassContaining{

    BlobState();
    BlobState(int pid, int sub_id);

    int serialize(char * buffer) const;
    int deserialize(char * buffer);
    std::ostream& print(std::ostream& s) const{
        s << "Blob ("<< id.sub_entity << ") - MASS:"<< mass << ", POS:" << pos << ", RAD:"<< radius << '\n';
        return s; 
    }

};

#endif // BLOB_STATE_H