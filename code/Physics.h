#ifndef OOP_PHYSICS_H
#define OOP_PHYSICS_H
#include "Pair.h"
#include <cmath>
#include <SFML/Graphics.hpp>

class Physics {
private:
    Pair position{};
    Pair velocity{};
    Pair acceleration{};
    double mass;
public:
    /// CONSTRUCTORS
    Physics(): position({0,0}), velocity({0,0}), acceleration({0,0}){}
    Physics(const Physics& state) {
        this->position = state.position;
        this->velocity = state.velocity;
        this->acceleration = state.acceleration;
    }
    explicit Physics(Pair position) {
        this->position = position;
        this->velocity = {0,0};
        this->acceleration = {0,0};
    }
    ~Physics() = default;
    Physics& operator=(const Physics& state) {
        this->position = state.position;
        this->velocity = state.velocity;
        this->acceleration = state.velocity;
        return *this;
    }
    void UpdatePhysics(float cap,sf::Time dt);
    void UpdatePosition(sf::Time dt);
    void FlickPhysics(Pair bulletspeed,Pair startacceleration,sf::Time dt);

    Pair getPosition() const;
    Pair getVelocity() const;
    Pair getAcceleration();
    void setAcceleration(Pair acceleration_);
    void setPosition(Pair position_);
    void setVelocity(Pair velocity_);
    double getMass();
    void addAcceleration(Pair accelerationtoadd);
    friend std::ostream& operator<<(std::ostream& out,const Physics& state);

    void resetAcceleration();
};
#endif //OOP_PHYSICS_H