#include "Physics.h"
#include "Pair.h"
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "ParticleSystem.h"
#include "SpaceShip.h"

#include <iostream>
#include "Collider.h"
#include "Exceptions.h"

void SpaceShip::ShipMove() {
    float angleRad = triangle.getRotation().asRadians();
    angleRad -= 3.14159265f / 2.f;
    Pair direction{std::cos(angleRad) * thrust, std::sin(angleRad) * thrust};
    physics.addAcceleration(direction);
}
void SpaceShip::ExhaustMove() {
    float angleRad = triangle.getRotation().asRadians() - 3.14159265f / 2.f;
    sf::Vector2f shipPos = triangle.getPosition();

    float backOffsetDist = triangle.getRadius() * 0.5f;
    sf::Vector2f offset(-std::cos(angleRad) * backOffsetDist,
                        -std::sin(angleRad) * backOffsetDist);

    float lateralOffset = triangle.getRadius() * 0.01f;
    sf::Vector2f lateral(-std::sin(angleRad) * lateralOffset,
                          std::cos(angleRad) * lateralOffset);

    exhaust.setEmitter(shipPos + offset + lateral);
    exhaust.setDirection(angleRad + 3.14159265f);
}

void SpaceShip::UpdateData(sf::Time dt,Pair newpos) {
    timer+=dt.asSeconds();
    distance_travelled+=sqrtf(pow(newpos.x-lastpos.x,2)+pow(newpos.y-lastpos.y,2));
    lastpos=newpos;
}

void SpaceShip::computeGravity(Pair position, double mass, double influenceRadius) {
    const double G = 1000.0;

    Pair shipPos = this->getPhysics().getPosition();
    double dx = position.x - shipPos.x;
    double dy = position.y - shipPos.y;

    double distSq = dx * dx + dy * dy;

    if (distSq < 100.f || distSq > influenceRadius * influenceRadius) {
        return;
    }
    double dist = std::sqrt(distSq);
    double accel = G * mass / distSq;
    if (std::isnan(dist) || std::isnan(accel)) {
        throw PhysicsException("Gravity computation produced NaN.");
    }
    physics.addAcceleration({ (dx / dist) * accel, (dy / dist) * accel });
}

void SpaceShip::ShootBullet() {
    energy-=10;
    Pair shipPos = physics.getPosition();
    float angleRad = triangle.getRotation().asRadians();
    angleRad -= 3.14159265f / 2.f;
    Pair bulletVelocity{std::cos(angleRad) * Bullet::speed, std::sin(angleRad) * Bullet::speed};
    Pair bulletPos{shipPos.x, shipPos.y};
    Physics bulletPhysics(bulletPos);
    Pair combined_velocity= {bulletVelocity.x+physics.getVelocity().x, bulletVelocity.y+physics.getVelocity().y};
    bulletPhysics.setVelocity(combined_velocity);

    Bullet newbullet(bulletPhysics);
    bullets.push_back(newbullet);
}
void SpaceShip::EnterHyper(sf::Time dt) {
    if (!inHyper) {
        hyperStartPos = physics.getPosition();
        showHyperTrail = true;
        float angleRad = triangle.getRotation().asRadians();
        angleRad -= 3.14159265f / 2.f;
        Pair direction{std::cos(angleRad) * hyper_thrust, std::sin(angleRad) * hyper_thrust};
        physics.setVelocity(direction);
    }
    ore-=10.f*dt.asSeconds();
    ore=std::max(0.f,static_cast<float>(ore));
    inHyper=true;
}
void SpaceShip::ExitHyper() {
    if (inHyper==true) {
        showHyperTrail = false;
        physics.setVelocity({0,0});
    }
    inHyper=false;
}
sf::Vector2f SpaceShip::SpaceToScreen(Pair world, Pair player, sf::RenderWindow& window) {
    const sf::Vector2f center = { window.getSize().x / 2.f, window.getSize().y / 2.f };
    return { center.x + (float)(world.x - player.x),
             center.y + (float)(world.y - player.y) };
}
void SpaceShip::UpdateHyperTrail() {
    if (!showHyperTrail) return;

    Pair shipPos = physics.getPosition();

    float dx = shipPos.x - hyperStartPos.x;
    float dy = shipPos.y - hyperStartPos.y;

    float length = std::sqrt(dx*dx + dy*dy);

    hyperTrail.setSize({ length, 6 });

    float angle = std::atan2(dy, dx) * 180.f / 3.14159265f;

    hyperTrail.setRotation(sf::degrees(angle));
    hyperTrail.setPosition({ (float)hyperStartPos.x, (float)hyperStartPos.y });
}
void SpaceShip::HyperLogic(sf::Time dt,sf::RenderWindow& window) {
    if (getHyper()==true && getOre()>0) {
        EnterHyper(dt);
        getPhysics().UpdatePosition(dt);
    }
    else {
        ExitHyper();
        getPhysics().UpdatePhysics(getCap(),dt);
    }
    UpdateHyperTrail();

    if (showHyperTrail) {
        sf::Vector2f screenStart = SpaceToScreen(hyperStartPos, getPhysics().getPosition(), window);
        hyperTrail.setPosition(screenStart);
        window.draw(hyperTrail);
    }
}
void SpaceShip::RefuelLogic(sf::Time dt,sf::RenderWindow& window) {
    fuel+=dt.asSeconds()*5.f;
    fuel = std::min(matscap,fuel);

    sf::Vector2f shipScreenPos = SpaceToScreen(physics.getPosition(), physics.getPosition(), window);
    sf::Vector2f starScreenPos = SpaceToScreen(StarPosrefuel, physics.getPosition(), window);

    sf::Vector2f delta = starScreenPos - shipScreenPos;
    float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    float angle = std::atan2(delta.y, delta.x) * 180.f / 3.14159265f;

    sf::RectangleShape fuelLine;
    fuelLine.setSize({length, 4.f});
    fuelLine.setOrigin({0.f, 2.f});
    fuelLine.setPosition(shipScreenPos);
    fuelLine.setRotation(sf::degrees(angle));
    fuelLine.setFillColor(sf::Color(0, 0, 255, 150));

    window.draw(fuelLine);
}
void SpaceShip::InputCheck(sf::Time dt,sf::RenderWindow& window) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && !inHyper) {
        triangle.rotate(sf::degrees(-250.f*dt.asSeconds()));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) && !inHyper){
        triangle.rotate(sf::degrees(250.f*dt.asSeconds()));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Z) && refuelpossible){
        RefuelLogic(dt, window);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::V) && ore>0.1f){
        EnterHyper(dt);
    }
    else {
        ExitHyper();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && fuel > 0) {
        ShipMove();
        ExhaustMove();
        fuel -= dt.asSeconds();
        fuel = std::max(0.f, static_cast<float>(fuel));
        upPressed = true;
    } else {
        upPressed = false;
    }

    exhaust.setEmitting(upPressed);

}
void SpaceShip::alignToPlanet(const Physics& planetPhys) {
    Pair shipPos = physics.getPosition();
    Pair planetPos = planetPhys.getPosition();

    double dx = shipPos.x - planetPos.x;
    double dy = shipPos.y - planetPos.y;
    double angle = std::atan2(dy, dx);

    triangle.setRotation(sf::degrees(angle * 180.0 / 3.1415926 + 90.0));
}
void SpaceShip::UpdateBullets(sf::Time dt,sf::RenderWindow& window,sf::FloatRect& viewRect) {
    energy+=dt.asSeconds()*5.f;
    energy=std::min(energy,matscap);
    for (long unsigned int i=0;i<bullets.size();i++) {
        bullets[i].Update(dt,getPhysics().getPosition(),window,viewRect);
        if (bullets[i].getLifetime()<0) {
            bullets.erase(bullets.begin()+i);
        }
    }
}

float SpaceShip::getFuel() const {
    return fuel;
}
float SpaceShip::getEnergy() const {
    return energy;
}
float SpaceShip::getOre() const {
    return ore;
}
Collider SpaceShip::getCollider() {
    return collider;
}
sf::CircleShape& SpaceShip::getShape() {
    return triangle;
}
double SpaceShip::getTimer() const {
    return timer;
}
double SpaceShip::getDistance_travelled() const{
    return distance_travelled;
}
Physics& SpaceShip::getPhysics() {
    return physics;
}
float SpaceShip::getCap() const {
    return cap;
}
ParticleSystem& SpaceShip::getExhaust() {
    return exhaust;
}
void SpaceShip::addOre(float value) {
    ore+=value;
    ore=std::min(ore,matscap);
}
std::vector<Bullet> &SpaceShip::getBullets() {
    return bullets;
}
std::ostream& operator<<(std::ostream& out,const SpaceShip& ship) {
    out<<"SHIP\n";
    out<<ship.physics<<'\n';
    out<<"Stats:\n";
    out<<"Fuel:"<<ship.fuel<<"Energy:"<<ship.energy<<"ORE:"<<ship.ore<<'\n';
    return out;
}
