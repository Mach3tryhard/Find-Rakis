#ifndef OOP_PLANET_H
#define OOP_PLANET_H

#include "Celestial.h"

class Planet : public Celestial {
public:
    static const std::vector<sf::Color> possibleColors;
    Planet(const Physics& physics): Celestial(physics, possibleColors[0]) {
    }

    void initialize(std::mt19937& gen) override;
    Planet* clone() const override { return new Planet(*this); }
};

#endif //OOP_PLANET_H