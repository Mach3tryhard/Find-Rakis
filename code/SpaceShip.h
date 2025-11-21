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
    Pair lastpos{};
    Collider collider;
    double fuel,energy,ore;
    double timer,distance_travelled;
    const double matscap=100;
    const float thrust = 120.0f;
    const float cap=350;
    bool upPressed=false;

    const float hyper_thrust=1000.0f;
    bool inHyper=false;
    sf::RectangleShape hyperTrail;
    bool showHyperTrail = false;
    Pair hyperStartPos{};

    bool refuelpossible;
    Pair StarPosrefuel{};
    double StarRadius;
    sf::Color refuelStarColor;
public:
    SpaceShip(const Physics& physics,float radius, double fuel, double energy, double ore):collider(radius/2) {
        hyperTrail.setFillColor(sf::Color(255, 255, 255, 200));
        hyperTrail.setSize({0, 3});
        hyperTrail.setOrigin({0, 1.5f});
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
        hyperStartPos={0,0};

        refuelpossible=false;
        StarPosrefuel={0,0};
        StarRadius=0;
        refuelStarColor=sf::Color(255, 255, 255, 150);
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
    void InputCheck(sf::Time dt, sf::RenderWindow &window);
    Collider getCollider();
    void EnterHyper(sf::Time dt);
    void ExitHyper();
    void HyperLogic(sf::Time dt, sf::RenderWindow &window);
    void UpdateHyperTrail();

    void RefuelLogic(sf::Time dt, sf::RenderWindow &window);
    void SetRefuel(bool canrefuel,Pair NewStarPos,double newStarRadius,sf::Color color) {
        refuelpossible=canrefuel;
        StarPosrefuel=NewStarPos;
        StarRadius=newStarRadius;
        refuelStarColor=color;
    }
    void UpdateData(sf::Time dt,Pair newpos);
    sf::Vector2f SpaceToScreen(Pair world,Pair player, sf::RenderWindow& window);
    void computeGravity(Pair position,double mass, double influenceRadius);
    friend std::ostream& operator<<(std::ostream& out,const SpaceShip& ship);
    void UpdateBullets(sf::Time dt,sf::RenderWindow& window,sf::FloatRect& viewRect);
    void alignToPlanet(const Physics& planetPhys);
};

#endif //OOP_SPACESHIP_H