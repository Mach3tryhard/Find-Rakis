#ifndef OOP_SPACESHIP_H
#define OOP_SPACESHIP_H
#include "Physics.h"
#include "Pair.h"
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "ParticleSystem.h"

class SpaceShip {
private:
    ParticleSystem exhaust{200};
    sf::CircleShape triangle{10.0f,3};
    std::vector<Bullet> bullets;
    Physics physics;
    const float thrust = 120.0f;
    double fuel,energy,ore;
    const float cap=300;
    bool upPressed=false;
public:
    SpaceShip(const Physics& physics, double fuel, double energy, double ore) {
        this->triangle.setOrigin({this->triangle.getRadius(), this->triangle.getRadius()});
        this->physics = physics;
        this->fuel = fuel;
        this->energy = energy;
        this->ore = ore;
    }
    sf::CircleShape& getShape();
    Physics& getPhysics();
    float getCap() const;
    ParticleSystem& getExhaust();
    std::vector<Bullet>& getBullets();
    void ShipMove();
    void ExhaustMove();
    void ShootBullet();
    void InputCheck(sf::Time dt);
    friend std::ostream& operator<<(std::ostream& out,const SpaceShip& ship);
};

#endif //OOP_SPACESHIP_H