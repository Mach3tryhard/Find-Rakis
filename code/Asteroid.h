#ifndef OOP_ASTEROID_H
#define OOP_ASTEROID_H
#include <SFML/Graphics.hpp>
#include <cstdint>
#include "Celestial.h"

class Asteroid : public Celestial {
private:
    struct Particle {
        sf::Vector2f offset;
        sf::Vector2f velocity;
    };
    std::vector<Particle> dustParticles;
public:
    static const std::vector<sf::Color> possibleColors;
    explicit Asteroid(const Physics& physics): Celestial(physics, sf::Color::White) {
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

    void CelestialEffects(sf::RenderWindow& window, Pair player, sf::FloatRect& viewRect) override;

    void initialize(std::mt19937 &gen, float orbitRadius) override;
    Asteroid* clone() const override { return new Asteroid(*this); }
};

#endif //OOP_ASTEROID_H