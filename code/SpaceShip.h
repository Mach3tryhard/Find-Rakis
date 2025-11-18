#ifndef OOP_SPACESHIP_H
#define OOP_SPACESHIP_H
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "ParticleSystem.h"
#include "Physics.h"
#include "Collider.h"
#include "Pair.h"

class SpaceShip {
private:
    ParticleSystem exhaust{1000};
    sf::CircleShape triangle;
    std::vector<Bullet> bullets;
    Physics physics;
    Pair lastpos;
    Collider collider;
    double fuel,energy,ore;
    double timer,distance_travelled;
    const double matscap=100;
    bool inHyper=false;
    const float thrust = 120.0f,hyper_thrust=1000.0f;
    const float cap=350;
    bool upPressed=false;
public:
    SpaceShip(const Physics& physics,float radius, double fuel, double energy, double ore):collider(radius/2) {
        this->triangle.setRadius(radius);
        this->triangle.setPointCount(3);
        this->triangle.setOrigin({radius, radius});
        this->physics = physics;
        this->fuel = fuel;
        this->energy = energy;
        this->ore = ore;
        this->timer = 0;
        this->distance_travelled = 0;
        this->lastpos = {0,0};
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
    double getTimer() const;
    bool getHyper() const{return inHyper;}
    double getDistance_travelled() const;
    void ShipMove();
    void ExhaustMove();
    void ShootBullet();
    void InputCheck(sf::Time dt);
    Collider getCollider();
    void EnterHyper();
    void ExitHyper();
    void DoHyper(sf::Time dt);
    void UpdateData(sf::Time dt,Pair newpos);
    void computeGravity(Pair position,double mass, double influenceRadius);
    friend std::ostream& operator<<(std::ostream& out,const SpaceShip& ship);
    void UpdateBullets(sf::Time dt,sf::RenderWindow& window,sf::FloatRect& viewRect);
    void alignToPlanet(const Physics& planetPhys);
};

#endif //OOP_SPACESHIP_H