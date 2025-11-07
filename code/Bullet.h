#ifndef OOP_BULLET_H
#define OOP_BULLET_H
#include "Pair.h"
#include <SFML/Graphics.hpp>
#include "Physics.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Bullet {
private:
    sf::CircleShape shape;
    Physics physics;
    const int damage=10;
    const int radius =5;
public:
    static const float speed;
    explicit Bullet(const Physics& physics) {
        shape.setRadius(this->radius);
        shape.setOrigin({static_cast<float>(this->radius), static_cast<float>(this->radius)});
        Pair pos = physics.getPosition();
        shape.setPosition({static_cast<float>(pos.x), static_cast<float>(pos.y)});
        shape.setFillColor(sf::Color::Green);
        this->physics = physics;
    }
    sf::CircleShape& getShape();
    Physics& getPhysics();
    void Display(const Pair& position, sf::RenderWindow& window, sf::FloatRect& viewRect);
    friend std::ostream& operator<<(std::ostream& out,const Bullet& bullet);
};
#endif //OOP_BULLET_H