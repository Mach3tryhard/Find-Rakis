#ifndef OOP_CELESTIAL_H
#define OOP_CELESTIAL_H
#include "Physics.h"
#include "Pair.h"
#include <SFML/Graphics.hpp>
#include "ParticleSystem.h"
#include "SpaceShip.h"

class Celestial {
private:
    sf::CircleShape shape;
    Physics physics;
    int health,index,color;
    double radius,gravity;
    bool solid;
public:
    Celestial(const Physics& physics,int index,sf::Color color) {
        ///RANDOMLY GENERATED CELESTIAL NOT DONE YET
        this->physics = physics;
        this->health = 10;
        this->index = index;
        this->radius = 150;
        this->gravity = 10;
        this->solid = true;
        shape.setRadius(this->radius);
        shape.setOrigin({static_cast<float>(this->radius), static_cast<float>(this->radius)});
        Pair pos = physics.getPosition();
        shape.setPosition({static_cast<float>(pos.x), static_cast<float>(pos.y)});
        shape.setFillColor(color);
    }
    Celestial(const Physics& physics,int health,double radius,double gravity,int color,bool solid,int index){
        this->physics = physics;
        this->health = health;
        this->radius = radius;
        this->gravity = gravity;
        this->color = color;
        this->solid = solid;
        this->index = index;
        shape.setRadius(this->radius);
        shape.setOrigin({static_cast<float>(this->radius), static_cast<float>(this->radius)});
        Pair pos = physics.getPosition();
        shape.setPosition({static_cast<float>(pos.x), static_cast<float>(pos.y)});
        shape.setFillColor(sf::Color::Blue);
    }
    double& getRadius();
    void Display(SpaceShip& player,sf::RenderWindow& window,sf::FloatRect& viewRect);
    sf::CircleShape& getShape();
    Physics& getPhysics();
    friend std::ostream& operator<<(std::ostream& out,const Celestial& body);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //OOP_CELESTIAL_H