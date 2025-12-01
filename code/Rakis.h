#ifndef OOP_RAKIS_H
#define OOP_RAKIS_H

#include <SFML/Graphics.hpp>
#include <cstdint>
#include "Celestial.h"

class Rakis : public Celestial {
private:
    float pulseCounter{};
    struct Particle {
        sf::Vector2f offset;
        sf::Vector2f velocity;
    };
    std::vector<Particle> dustParticles;
public:
    static const std::vector<sf::Color> possibleColors;
    explicit Rakis(const Physics& physics): Celestial(physics, sf::Color::White) {
        std::mt19937 gen(std::random_device{}());
        std::uniform_real_distribution<float> dist(-200.f, 200.f);
        std::uniform_real_distribution<float> velDist(-0.02f, 0.02f);

        for (int i = 0; i < 20; ++i) {
            dustParticles.push_back({
                {dist(gen), dist(gen)},
                {velDist(gen), velDist(gen)}
            });
        }
    }
    bool isRakis()override{return true}
    void CelestialEffects(sf::RenderWindow& window, Pair player, sf::FloatRect& viewRect) override;
    void isCrashed(SpaceShip& player) override;
    void initialize(std::mt19937 &gen, float orbitRadius) override;
    Rakis* clone() const override { return new Rakis(*this); }
};

#endif //OOP_RAKIS_H