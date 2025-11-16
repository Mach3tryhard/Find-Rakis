#include "Asteroid.h"

const std::vector<sf::Color> Asteroid::possibleColors= {sf::Color(0x964B00FF),sf::Color(0x964B00FF),sf::Color(0x964B00FF)};

void Asteroid::initialize(std::mt19937& gen) {
    std::uniform_real_distribution<> distrib_radius(80.0, 300.0);
    std::uniform_real_distribution<> distrib_mass(500.0, 2000.0);
    std::uniform_int_distribution<> distrib_color(0, 2);

    double radius = distrib_radius(gen);
    this->getPhysics().setMass(distrib_mass(gen));
    this->collider = Collider(radius);
    health = 100;

    shape.setRadius(radius);
    shape.setOrigin({static_cast<float>(radius), static_cast<float>(radius)});
    shape.setFillColor(possibleColors[distrib_color(gen)]);
    Pair pos = physics.getPosition();
    shape.setPosition({static_cast<float>(pos.x), static_cast<float>(pos.y)});
}
void Asteroid::CelestialEffects(sf::RenderWindow& window, Pair player, sf::FloatRect& viewRect) {
    Pair pos = physics.getPosition();
    const sf::Vector2f screenCenter = { window.getSize().x / 2.f, window.getSize().y / 2.f };
    float screenX = screenCenter.x + static_cast<float>(pos.x - player.x);
    float screenY = screenCenter.y + static_cast<float>(pos.y - player.y);

    float radius = getRadius();

    if (!ToDisplay(screenX, screenY, radius + 200.f, viewRect)) return;

    for (auto& p : dustParticles) {
        p.offset += p.velocity;

        if (std::abs(p.offset.x) > 200.f) p.velocity.x = -p.velocity.x;
        if (std::abs(p.offset.y) > 200.f) p.velocity.y = -p.velocity.y;

        sf::CircleShape dust(3.f);
        dust.setOrigin({3.f, 3.f});
        dust.setPosition({screenX + p.offset.x, screenY + p.offset.y});
        dust.setFillColor(shape.getFillColor());

        window.draw(dust);
    }
}