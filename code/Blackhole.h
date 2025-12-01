#ifndef OOP_BLACKHOLE_H
#define OOP_BLACKHOLE_H

#include <SFML/Graphics.hpp>
#include <cstdint>
#include "Celestial.h"

class Blackhole : public Celestial {
private:
    float pulseTimer = 0.f;
    float rotationAngle{};
public:
    static const std::vector<sf::Color> possibleColors;
    explicit Blackhole(const Physics& physics): Celestial(physics, sf::Color::White) {
    }
    bool isRakis()override{return false;}
    void CelestialEffects(sf::RenderWindow& window, Pair player, sf::FloatRect& viewRect) override;
    void isCrashed(SpaceShip& player) override;
    void initialize(std::mt19937 &gen, float orbitRadius) override;
    Blackhole* clone() const override { return new Blackhole(*this); }
};

#endif //OOP_BLACKHOLE_H