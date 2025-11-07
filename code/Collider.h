#ifndef OOP_COLLIDER_H
#define OOP_COLLIDER_H
#include "SFML/Graphics/CircleShape.hpp"
#include "Physics.h"

class Collider {
private:
    double radius;
    sf::CircleShape debugShape;
public:
    Collider(double radius, const sf::CircleShape &debug_shape)
        : radius(radius),
          debugShape(debug_shape) {
    }
    ~Collider() = default;
    bool isCollidingWith(Physics thisf,Physics thatf,Collider collider);
    Pair addNormalVector(Physics ship);
    double getRadius();
};

#endif //OOP_COLLIDER_H