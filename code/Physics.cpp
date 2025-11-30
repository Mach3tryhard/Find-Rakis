#include "Physics.h"
#include "Pair.h"
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Exceptions.h"

void Physics::OrbitBody(const Pair& center, sf::Time dt) {
    double dx = position.x - center.x;
    double dy = position.y - center.y;

    double radius = std::sqrt(dx*dx + dy*dy);
    if (radius < 0.0001) return;

    double angle = std::atan2(dy, dx);

    double angularVel = 100.0 / radius;

    double newAngle = angle + angularVel * dt.asSeconds();

    double newX = center.x + radius * std::cos(newAngle);
    double newY = center.y + radius * std::sin(newAngle);

    double velX = (newX - position.x) / dt.asSeconds();
    double velY = (newY - position.y) / dt.asSeconds();

    position.x = newX;
    position.y = newY;
    velocity = { velX, velY };
}


void Physics::UpdatePhysics(float cap,sf::Time dt) {
    float delta = dt.asSeconds();
    if (std::isnan(velocity.x) || std::isnan(velocity.y)) {
        return;
    }
    this->velocity.x += this->acceleration.x * delta;
    this->velocity.y += this->acceleration.y * delta;

    float len = sqrtf(velocity.x*velocity.x + velocity.y*velocity.y);
    if (len > cap && len > 0.0001f) {
        this->velocity.x = (velocity.x / len) * cap;
        this->velocity.y = (velocity.y / len) * cap;
    }
    this->position.x += this->velocity.x * delta;
    this->position.y += this->velocity.y * delta;
    acceleration = {0.0, 0.0};
    if (std::isnan(velocity.x) || std::isnan(velocity.y)) {
        throw PhysicsException("Velocity este nan");
    }
}
void Physics::UpdatePosition(sf::Time dt) {
    float delta = dt.asSeconds();
    this->position.x += this->velocity.x * delta;
    this->position.y += this->velocity.y * delta;
}
void Physics::addAcceleration(Pair accelerationtoadd) {
    this->acceleration.x += accelerationtoadd.x;
    this->acceleration.y += accelerationtoadd.y;
}
Pair Physics::getPosition() const {
    return position;
}
Pair Physics::getVelocity() const {
    return velocity;
}
Pair Physics::getAcceleration(){
    return acceleration;
}
double Physics::getMass() {
    return mass;
}
void Physics::setMass(double mass_) {
    this->mass = mass_;
}
void Physics::setPhysics(const Physics& newPhysics) {
    this->mass = newPhysics.mass;
    this->velocity = newPhysics.velocity;
    this->acceleration = newPhysics.acceleration;
    this->position = newPhysics.position;
}
void Physics::setPosition(Pair position_) {
    this->position = position_;
}
void Physics::setVelocity(Pair velocity_) {
    this->velocity = velocity_;
}
std::ostream& operator<<(std::ostream& out, const Physics& state) {
    out<<"Position:X-"<<state.position.x<<" Y-"<<state.position.y<<'\n';
    out<<"Velocity:X-"<<state.velocity.x<<" Y-"<<state.velocity.y<<'\n';
    return out;
}