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
    Physics(): position({0,0}), velocity({0,0}), acceleration({0,0}),mass{0} {
    }
    explicit Physics(double mass): position({0,0}), velocity({0,0}), acceleration({0,0}) {
        this->mass = mass;
    }
    Physics(const Physics& state) {
        this->position = state.position;
        this->velocity = state.velocity;
        this->acceleration = state.acceleration;
        this->mass = state.mass;
    }
    explicit Physics(Pair position) {
        this->position = position;
        this->velocity = {0,0};
        this->acceleration = {0,0};
        this->mass = 0;
    }
    ~Physics() = default;
    Physics& operator=(const Physics& state) {
        this->position = state.position;
        this->velocity = state.velocity;
        this->acceleration = state.acceleration;
        this->mass = state.mass;
        return *this;
    }

    void OrbitBody(const Pair &center, sf::Time dt);
    void UpdatePhysics(float cap,sf::Time dt);
    void UpdatePosition(sf::Time dt);

    Pair getPosition() const;
    Pair getVelocity() const;
    Pair getAcceleration();
    void setPosition(Pair position_);
    void setVelocity(Pair velocity_);
    double getMass();

    void setMass(double mass);

    void addAcceleration(Pair accelerationtoadd);
    void setPhysics(const Physics& newPhysics);
    void setAcceleration(Pair Accel) {acceleration=Accel;}
    friend std::ostream& operator<<(std::ostream& out,const Physics& state);
};
#endif //OOP_PHYSICS_H