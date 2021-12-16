#include "state/BlobState.h"
#include <math.h>
#include "core/macros.h"
int BlobState::serialize(char * buffer){
    int bytes = 0;
    std::memcpy(buffer,&mass,sizeof(unsigned int));
    buffer += sizeof(unsigned int);
    bytes += sizeof(unsigned int);

    bytes += pos.serialize(buffer);
    return bytes;
}

int BlobState::deserialize(char * buffer){
    int bytes = 0;
    std::memcpy(&mass,buffer,sizeof(unsigned int));
    buffer += sizeof(unsigned int);
    bytes += sizeof(unsigned int);

    bytes += pos.deserialize(buffer);
    return bytes;
}

void BlobState::draw(sf::RenderWindow * rw){
    auto radius = pow(mass* 0.978394833 + 1.405197198 ,0.485133) * 0.228728 *  CLIENT_UNIT_LENGTH;
    sf::CircleShape circle(radius);
    circle.setPointCount(20);
    circle.setOrigin(radius,radius);
    circle.setPosition(pos[0],pos[1]);
    rw->draw(circle);
}

bool BlobState::collides(ICollidable& o, Hit& h) {
    Vec2 diff = (o.get_pos() - pos);
    float len = diff.norm();
    float radi_sum = (radius + o.get_radius());
    if( len <= radi_sum){
        h.entity1 = get_ID();
        h.entity2 = o.get_ID();
        diff.normalize();
        h.pos = pos + (diff * radius);
        h.normal = diff;
        h.overlap = 1-(len / (std::min(radius, o .get_radius()) * 2)); // overlap in terms of smaller radius
        return true;
    } else {
        return false;
    }
}
