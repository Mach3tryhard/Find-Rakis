#include "Physics.h"
#include "Pair.h"
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "ParticleSystem.h"
#include "SpaceShip.h"
#include "Collider.h"

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

void SpaceShip::computeGravity(Pair position, double mass, double influenceRadius) {
    const double G = 1000.0;

    Pair shipPos = this->getPhysics().getPosition();
    double dx = position.x - shipPos.x;
    double dy = position.y - shipPos.y;

    double distSq = dx * dx + dy * dy;

    if (distSq < 100 || distSq > influenceRadius * influenceRadius) {
        physics.addAcceleration({ 0, 0 });
        return;
    }
    double dist = std::sqrt(distSq);
    double accel = G * mass / distSq;
    physics.addAcceleration({ (dx / dist) * accel, (dy / dist) * accel });
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
    }else{
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
    for (auto i:bullets) {
        i.Update(dt,getPhysics().getPosition(),window,viewRect);
    }
}
Collider SpaceShip::getCollider() {
    return collider;
}
sf::CircleShape& SpaceShip::getShape() {
    return triangle;
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
std::ostream& operator<<(std::ostream& out,const SpaceShip& ship) {
    out<<"SHIP\n";
    out<<ship.physics<<'\n';
    out<<"Stats:\n";
    out<<"Fuel:"<<ship.fuel<<"Energy:"<<ship.energy<<"ORE:"<<ship.ore<<'\n';
    return out;
}
