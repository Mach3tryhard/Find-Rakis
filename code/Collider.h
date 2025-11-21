#ifndef OOP_COLLIDER_H
#define OOP_COLLIDER_H
#include <SFML/Graphics.hpp>
#include "Physics.h"

class Collider {
private:
    double radius;
    sf::CircleShape debugShape;
public:
    explicit Collider(double radius) {
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

    bool isCollidingWith(const Physics &thisf, const Physics &thatf, Collider &other);
    double getRadius();

    //void drawDebug(sf::RenderWindow& window, const Physics& physics);
    Physics resolveCollision(const Physics& shipPhys, const Physics& planetPhys, double planetRadius)  const;
};


#endif //OOP_COLLIDER_H