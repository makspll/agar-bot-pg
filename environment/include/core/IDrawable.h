#ifndef ENTITY_H
#define ENTITY_H
#include <SFML/Graphics.hpp>

class IDrawable {

    virtual void draw(sf::RenderWindow * rw) const = 0;

};


#endif //ENTITY_H