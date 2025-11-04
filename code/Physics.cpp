#include "Physics.h"
#include "Pair.h"
#include <cmath>
#include <SFML/Graphics.hpp>

void Physics::UpdatePhysics(float cap,sf::Time dt) {
    float delta = dt.asSeconds();
    this->velocity.x += this->acceleration.x * delta;
    this->velocity.y += this->acceleration.y * delta;

    float len = sqrtf(velocity.x*velocity.x + velocity.y*velocity.y);
    if (len > cap) {
        this->velocity.x = velocity.x / len * cap;
        this->velocity.y = velocity.y / len * cap;
    }
    this->position.x += this->velocity.x * delta;
    this->position.y += this->velocity.y * delta;
}
void Physics::UpdatePosition(sf::Time dt) {
    float delta = dt.asSeconds();
    this->position.x += this->velocity.x * delta;
    this->position.y += this->velocity.y * delta;
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
void Physics::setAcceleration(Pair acceleration_) {this->acceleration = acceleration_;}
void Physics::setPosition(Pair position_) {this->position = position_;}
void Physics::setVelocity(Pair velocity_) {this->velocity = velocity_;}
std::ostream& operator<<(std::ostream& out, const Physics& state) {
    out<<"Position:X-"<<state.position.x<<" Y-"<<state.position.y<<'\n';
    out<<"Velocity:X-"<<state.velocity.x<<" Y-"<<state.velocity.y<<'\n';
    return out;
}