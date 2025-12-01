#ifndef OOP_STAR_H
#define OOP_STAR_H

#include "Celestial.h"

class Star : public Celestial {
public:
    static const std::vector<sf::Color> possibleColors;
    explicit Star(const Physics& physics) : Celestial(physics, sf::Color::White) {
    }

    void CelestialEffects(sf::RenderWindow& window, Pair player, sf::FloatRect& viewRect) override;

    void RefuelCheck(SpaceShip &player) override;
    void isCrashed(SpaceShip& player) override;
    void initialize(std::mt19937 &gen, float orbitRadius) override;

    bool isRakis()override {return false;}

    Star* clone() const override { return new Star(*this); }
};

#endif //OOP_STAR_H