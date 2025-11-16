#ifndef OOP_BULLET_H
#define OOP_BULLET_H
#include "Pair.h"
#include <SFML/Graphics.hpp>
#include "Collider.h"
#include "Physics.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Bullet {
private:
    sf::CircleShape shape;
    Physics physics;
    Collider collider;
    float damage=25;
    float radius =5;
    float lifetime=10;
public:
    static const float speed;
    explicit Bullet(const Physics& physics) : collider(5.f) {
        shape.setRadius(this->radius);
        shape.setOrigin({static_cast<float>(this->radius), static_cast<float>(this->radius)});
        Pair pos = physics.getPosition();
        shape.setPosition({static_cast<float>(pos.x), static_cast<float>(pos.y)});
        shape.setFillColor(sf::Color::Green);
        this->physics = physics;
    }

    sf::CircleShape& getShape();
    Physics& getPhysics();
    float getDamage() const;
    float getLifetime() const;

    Collider &getCollider();
    void Update(sf::Time dt,Pair player,sf::RenderWindow& window,sf::FloatRect& viewRects);
    void Display(const Pair& position, sf::RenderWindow& window, sf::FloatRect& viewRect);
    Bullet(const Bullet& other);
    Bullet &operator=(const Bullet &other);
    friend std::ostream& operator<<(std::ostream& out,const Bullet& bullet);
};
#endif //OOP_BULLET_H