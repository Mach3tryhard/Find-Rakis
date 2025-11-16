#ifndef OOP_PLANET_H
#define OOP_PLANET_H

#include "Celestial.h"

class Planet : public Celestial {
    float pulseCounter;
public:
    static const std::vector<sf::Color> possibleColors;
    explicit Planet(const Physics& physics): Celestial(physics, sf::Color::White) {
    }

    void CelestialEffects(sf::RenderWindow& window, Pair player, sf::FloatRect& viewRect) override;

    void initialize(std::mt19937& gen) override;
    Planet* clone() const override { return new Planet(*this); }
};

#endif //OOP_PLANET_H