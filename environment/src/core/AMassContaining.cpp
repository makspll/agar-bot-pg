#include "core/AMassContaining.h"


AMassContaining::AMassContaining() : ACollidable(-1, -1,0,Vec2(ARENA_HALF_WIDTH_UNITS*CLIENT_UNIT_LENGTH * 2,ARENA_HALF_WIDTH_UNITS*CLIENT_UNIT_LENGTH * 2)){
    set_radius_from_mass();
}

void AMassContaining::set_mass(unsigned int mass){
    this->mass = mass;
    set_radius_from_mass();
};

void AMassContaining::draw(sf::RenderWindow * rw) const{
    sf::CircleShape circle(radius);
    circle.setPointCount(20);
    circle.setOrigin(radius,radius);
    circle.setPosition(pos[0],pos[1]);
    rw->draw(circle);
};


bool AMassContaining::collides(ICollidable& o, Hit& h){
    Vec2 diff = (o.get_pos() - pos);
    float len = diff.norm();
    float radi_sum = (radius + o.get_radius());
    if( len <= radi_sum){
        h.entity1 = get_ID();
        h.entity2 = o.get_ID();
        diff.normalize();
        h.pos = pos + (diff * radius);
        h.normal = diff;
        h.overlap = (radius -len + o.get_radius()) / (std::min(radius, o .get_radius()) * 2); // overlap in terms of smaller radius
        return true;
    } else {
        return false;
    }
};

float AMassContaining::get_curr_radius(){
    return pow(mass* 0.978394833 + 1.405197198 ,0.485133) * 0.228728 *  CLIENT_UNIT_LENGTH;
};

float AMassContaining::set_radius_from_mass(){
    radius = get_curr_radius();
    return radius;
};
