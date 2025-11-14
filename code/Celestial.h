#ifndef OOP_CELESTIAL_H
#define OOP_CELESTIAL_H
#include <iostream>

#include "Physics.h"
#include "Pair.h"
#include <SFML/Graphics.hpp>
#include "ParticleSystem.h"
#include "SpaceShip.h"
#include "Collider.h"

class Celestial {
protected:
    sf::CircleShape shape;
    Physics physics;
    int health,color;
    bool solid;
    Collider collider;
public:
    Celestial(const Physics& physics,sf::Color color) : collider(0) {
        ///RANDOMLY GENERATED CELESTIAL NOT DONE YET
        this->physics = physics;
        this->health = 10;
        this->solid = true;
        shape.setRadius(static_cast<float>(0));
        shape.setOrigin({static_cast<float>(0), static_cast<float>(0)});
        Pair pos = physics.getPosition();
        shape.setPosition({static_cast<float>(pos.x), static_cast<float>(pos.y)});
        shape.setFillColor(color);
    }

    virtual void initialize(std::mt19937& gen) = 0;

    Celestial(const Physics& physics,int health,double radius,int color,bool solid) : collider(radius) {
        this->physics = physics;
        this->health = health;
        this->color = color;
        this->solid = solid;
        shape.setRadius(radius);
        shape.setOrigin({static_cast<float>(radius), static_cast<float>(radius)});
        Pair pos = physics.getPosition();
        shape.setPosition({static_cast<float>(pos.x), static_cast<float>(pos.y)});
        shape.setFillColor(sf::Color::Blue);
    }

    void CheckHit(std::vector<Bullet>& bullet,std::vector<Celestial*>& celestials,int ind);
    void LoseHealth(float damage,std::vector<Celestial*>& celestials,int ind);
    Collider &getCollider();
    double getRadius();
    void computeGravity(SpaceShip& player);
    void Display(Pair player, sf::RenderWindow &window, sf::FloatRect &viewRect, sf::Texture &texture);
    sf::CircleShape& getShape();
    Physics& getPhysics();
    virtual ~Celestial() = default;
    virtual Celestial* clone() const = 0;
    friend std::ostream& operator<<(std::ostream& out,const Celestial& body);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: CEva cu factory la solar system constructor ca sa apelez diferit idk si o functie virtuala cu functionalitate diferita nu doar generare

#endif //OOP_CELESTIAL_H