#ifndef OOP_COLLIDER_H
#define OOP_COLLIDER_H
#include "SFML/Graphics/CircleShape.hpp"
#include "Physics.h"

class Collider {
private:
    double radius;
    sf::CircleShape debugShape;
public:
    Collider(double radius) {
        this->radius = radius;
        debugShape.setRadius(radius);
        debugShape.setOrigin({static_cast<float>(radius), static_cast<float>(radius)});
        debugShape.setFillColor(sf::Color(0, 0, 0, 0));
        debugShape.setOutlineColor(sf::Color::Green);
        debugShape.setOutlineThickness(1.0f);
    }
    Collider(double radius, const sf::CircleShape &debug_shape)
        : radius(radius),
          debugShape(debug_shape) {
    }
    ~Collider() = default;
    bool isCollidingWith(Physics& thisf,Physics& thatf,Collider& collider);
    double getRadius();
    void drawDebug(sf::RenderWindow& window, const Physics& physics);
    Physics resolveCollision(const Physics &shipPhys, const Physics &planetPhys);
};


#endif //OOP_COLLIDER_H