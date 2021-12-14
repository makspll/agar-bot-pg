#ifndef BLOB_STATE_H
#define BLOB_STATE_H
#include "math/math.h"
#include "core/IDrawable.h"
#include "core/IPrintable.h"

struct BlobState : ISerializable, IDrawable, IPrintable{

    int serialize(char * buffer);
    int deserialize(char * buffer);
    void draw(sf::RenderWindow * rw);

    std::ostream& print(std::ostream& s) const{
        s << "MASS:"<< mass << ", POS:" << pos << '\n'; 
    }

    Vec2 pos;
    unsigned int mass;

};

#endif // BLOB_STATE_H