#ifndef OOP_ASTEROID_H
#define OOP_ASTEROID_H

#include "Celestial.h"

class Asteroid : public Celestial {
public:
    static const std::vector<sf::Color> possibleColors;
    Asteroid(const Physics& physics): Celestial(physics, possibleColors[0]) {
    }

    void initialize(std::mt19937& gen) override;
    Asteroid* clone() const override { return new Asteroid(*this); }
};

#endif //OOP_ASTEROID_H