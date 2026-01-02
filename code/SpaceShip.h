#ifndef OOP_SPACESHIP_H
#define OOP_SPACESHIP_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "ParticleSystem.h"
#include "Physics.h"
#include "Collider.h"
#include "Pair.h"
#include <SFML/Audio.hpp>

#include "Exceptions.h"
#include "ShipComputer.h"

class SpaceShip {
private:
    ParticleSystem exhaust{1000};
    sf::ConvexShape triangle;
    std::vector<Bullet> bullets;
    Physics physics;
    Pair lastpos{};
    Collider collider;
    double fuel,energy,ore;
    double timer,distance_travelled;
    const double matscap=100;
    const float thrust = 120.0f;
    const float cap=450;
    bool upPressed=false,DEAD=false,WON=false;

    const float hyper_thrust=1000.0f;
    bool inHyper=false;
    sf::RectangleShape hyperTrail;
    bool showHyperTrail = false;
    Pair hyperStartPos{};

    bool refuelpossible;
    Pair StarPosrefuel{};
    double StarRadius;
    sf::Color refuelStarColor;

    sf::SoundBuffer engineBuffer;
    sf::SoundBuffer laserBuffer;
    sf::SoundBuffer refuelBuffer;
    sf::SoundBuffer hyperBuffer;
    sf::SoundBuffer explosionBuffer;
    sf::Sound engineSound;
    sf::Sound laserSound;
    sf::Sound refuelSound;
    sf::Sound hyperSound;
    sf::Sound explosionSound;

    bool wasRotatingLeft = false;
    bool wasRotatingRight = false;
    bool wasThrusting = false;
    bool wasRefueling = false;
public:
    SpaceShip(const Physics& physics,float radius, double fuel, double energy, double ore)
        : collider(radius / 2), engineSound(engineBuffer), laserSound(laserBuffer), refuelSound(refuelBuffer),
          hyperSound(hyperBuffer), explosionSound(explosionBuffer) {
        if (!engineBuffer.loadFromFile("audio/engine3.wav")) {throw ResourceLoadException("audio/engine3.wav lipseste");}
        if (!laserBuffer.loadFromFile("audio/laser.wav")) {throw ResourceLoadException("audio/laser.wav lipseste");}
        if (!refuelBuffer.loadFromFile("audio/refuel.wav")) {throw ResourceLoadException("audio/refuel.wav lipseste");}
        if (!hyperBuffer.loadFromFile("audio/powerUp.wav")) {throw ResourceLoadException("audio/powerUp.wav lipseste");}
        if (!explosionBuffer.loadFromFile("audio/explosion.wav")) {throw ResourceLoadException("audio/explosion.wav lipseste");}
        laserSound.setBuffer(laserBuffer);
        laserSound.setVolume(40.f);
        laserSound.setPitch(1.0f);
        refuelSound.setBuffer(refuelBuffer);
        refuelSound.setVolume(20.f);
        refuelSound.setPitch(2.0f);
        hyperSound.setBuffer(hyperBuffer);
        hyperSound.setVolume(20.f);

        hyperTrail.setFillColor(sf::Color(255, 255, 255, 200));
        hyperTrail.setSize({0, 3});
        hyperTrail.setOrigin({0, 1.5f});
        triangle.setPointCount(3);
        float size = 10.f;
        triangle.setPoint(0, sf::Vector2f(0.f, -size));      // Nose (Up)
        triangle.setPoint(1, sf::Vector2f(-size/1.5f, size)); // Bottom Left
        triangle.setPoint(2, sf::Vector2f(size/1.5f, size));  // Bottom Right
        triangle.setFillColor(sf::Color::White);
        this->physics = physics;
        this->fuel = fuel;
        this->energy = energy;
        this->ore = ore;
        this->timer = 0;
        this->distance_travelled = 0;
        this->lastpos = {0, 0};
        hyperStartPos = {0, 0};
        this->DEAD = false;

        refuelpossible = false;
        StarPosrefuel = {0, 0};
        StarRadius = 0;
        refuelStarColor = sf::Color(255, 255, 255, 150);
    }

    //sf::CircleShape& getShape(){return triangle;}
    sf::ConvexShape& getShape() {
        return triangle;
    }
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
    void InputCheck(sf::Time dt, sf::RenderWindow &window, ShipComputer &computer);
    Collider getCollider();
    void EnterHyper(sf::Time dt);
    void ExitHyper();
    void HyperLogic(sf::Time dt, sf::RenderWindow &window);
    void UpdateHyperTrail();

    void RefuelLogic(sf::Time dt, sf::RenderWindow &window);
    void StopAudio() {
        engineSound.pause();
        laserSound.pause();
        refuelSound.pause();
        hyperSound.pause();
    }
    void SetRefuel(bool canrefuel,Pair NewStarPos,double newStarRadius,sf::Color color) {
        refuelpossible=canrefuel;
        StarPosrefuel=NewStarPos;
        StarRadius=newStarRadius;
        refuelStarColor=color;
    }
    void UpdateData(sf::Time dt,Pair newpos);
    void setDead(bool _dead) {
        DEAD=_dead;
    }
    bool getDead() {
        return DEAD;
    }
    void setWon(bool _won) {
        WON=_won;
    }
    bool getWon() {
        return WON;
    }
    sf::Vector2f SpaceToScreen(Pair world,Pair player, sf::RenderWindow& window);
    void computeGravity(Pair position,double mass, double influenceRadius);
    friend std::ostream& operator<<(std::ostream& out,const SpaceShip& ship);
    void UpdateBullets(sf::Time dt,sf::RenderWindow& window,sf::FloatRect& viewRect);
    void alignToPlanet(const Physics& planetPhys);

    void reset(sf::Vector2f center);
    void playExplosion() {
        explosionSound.play();
    }
    void setLast(Pair position) {
        lastpos=position;
    }
};

#endif //OOP_SPACESHIP_H