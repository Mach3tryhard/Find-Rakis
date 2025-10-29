#include "Physics.h"
#include "Pair.h"
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "ParticleSystem.h"
#include "SpaceShip.h"

sf::CircleShape& SpaceShip::getShape() { return triangle; }
    Physics& SpaceShip::getPhysics() { return physics; }
    float SpaceShip::getCap() const { return cap; }
    ParticleSystem& SpaceShip::getExhaust() { return exhaust;}
    std::vector<Bullet>& SpaceShip::getBullets() { return bullets; }
    void SpaceShip::ShipMove() {
        float angleRad = triangle.getRotation().asRadians();
        angleRad -= 3.14159265f / 2.f;
        Pair direction{std::cos(angleRad) * thrust, std::sin(angleRad) * thrust};
        physics.setAcceleration(direction);
    }
    void SpaceShip::ExhaustMove() {
        float angleRad = triangle.getRotation().asRadians();
        float shipRotationDeg = triangle.getRotation().asDegrees();
        sf::Vector2f shipPos(triangle.getPosition().x,triangle.getPosition().y);
        sf::Vector2f offset(-std::cos(angleRad) * 2.f, -std::sin(angleRad) * 2.f);
        float emissionDeg = shipRotationDeg - 90.f + 180.f; // backward from ship nose
        float emissionRad = emissionDeg * 3.14159265f / 180.f;
        exhaust.setEmitter(shipPos + offset);
        exhaust.setDirection(emissionRad);
    }
    void SpaceShip::ShootBullet() {
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
    void SpaceShip::InputCheck(sf::Time dt) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
            triangle.rotate(sf::degrees(-250.f*dt.asSeconds()));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right)){
            triangle.rotate(sf::degrees(250.f*dt.asSeconds()));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            upPressed = true;
            ShipMove();
            ExhaustMove();
        }else
        if (upPressed) {
            upPressed = false;
            physics.setAcceleration({0, 0});
        }
        exhaust.setEmitting(upPressed);
    }

std::ostream& operator<<(std::ostream& out,const SpaceShip& ship) {
    out<<"SHIP\n";
    out<<ship.physics<<'\n';
    out<<"Stats:\n";
    out<<"Fuel:"<<ship.fuel<<"Energy:"<<ship.energy<<"ORE:"<<ship.ore<<'\n';
    return out;
}