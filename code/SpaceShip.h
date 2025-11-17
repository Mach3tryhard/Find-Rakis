#ifndef OOP_SPACESHIP_H
#define OOP_SPACESHIP_H
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "ParticleSystem.h"
#include "Physics.h"
#include "Collider.h"

class SpaceShip {
private:
    ParticleSystem exhaust{1000};
    sf::CircleShape triangle;
    std::vector<Bullet> bullets;
    Physics physics;
    Collider collider;
    double fuel,energy,ore;
    const double matscap=100;
    const float thrust = 120.0f;
    const float cap=350;
    bool upPressed=false;
public:
    SpaceShip(const Physics& physics,float radius, double fuel, double energy, double ore):collider(radius/2) {
        this->triangle.setRadius(radius);
        this->triangle.setPointCount(3);
        this->triangle.setOrigin({static_cast<float>(radius), static_cast<float>(radius)});
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
    void addOre(float value);
    float getFuel() const;
    float getEnergy() const;
    float getOre() const;
    void ShipMove();
    void ExhaustMove();
    void ShootBullet();
    void InputCheck(sf::Time dt);
    Collider getCollider();
    void snapToPlanetSurface(const Physics& planetPhys, double planetRadius);
    void computeGravity(Pair position,double mass, double influenceRadius);
    friend std::ostream& operator<<(std::ostream& out,const SpaceShip& ship);
    void UpdateBullets(sf::Time dt,sf::RenderWindow& window,sf::FloatRect& viewRect);
    void alignToPlanet(const Physics& planetPhys);
};

#endif //OOP_SPACESHIP_H