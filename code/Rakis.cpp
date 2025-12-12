#include "Rakis.h"

const std::vector<sf::Color> Rakis::possibleColors= {sf::Color(0xd7ac89FF),sf::Color(0x0236e0FF)};

void Rakis::initialize(std::mt19937& gen,float _orbitRadius) {
    std::uniform_real_distribution<> distrib_radius(120.0, 130.0);
    std::uniform_real_distribution<> distrib_mass(250.0, 300.0);

    double radius = distrib_radius(gen);
    this->getPhysics().setMass(distrib_mass(gen));
    this->collider = Collider(radius);
    this->orbitRadius = _orbitRadius;
    health = 100000;

    shape.setRadius(radius);
    shape.setOrigin({static_cast<float>(radius), static_cast<float>(radius)});
    shape.setFillColor(possibleColors[0]);
    Pair pos = physics.getPosition();
    shape.setPosition({static_cast<float>(pos.x), static_cast<float>(pos.y)});
}
void Rakis::isCrashed(SpaceShip& player) {
    player.setWon(true);
}
void Rakis::CelestialEffects(sf::RenderWindow& window, Pair player, sf::FloatRect& viewRect) {
    Pair pos = physics.getPosition();
    const sf::Vector2f screenCenter = { window.getSize().x / 2.f, window.getSize().y / 2.f };
    float screenX = screenCenter.x + static_cast<float>(pos.x - player.x);
    float screenY = screenCenter.y + static_cast<float>(pos.y - player.y);

    float radius = getRadius();

    if (!ToDisplay(screenX, screenY, radius + 100.f, viewRect)) return;

    for (auto& p : dustParticles) {
        p.offset += p.velocity;

        if (std::abs(p.offset.x) > 100.f) p.velocity.x = -p.velocity.x;
        if (std::abs(p.offset.y) > 100.f) p.velocity.y = -p.velocity.y;

        sf::CircleShape dust(3.f);
        dust.setOrigin({3.f, 3.f});
        dust.setPosition({screenX + p.offset.x, screenY + p.offset.y});
        dust.setFillColor(possibleColors[1]);

        window.draw(dust);
    }

    pulseCounter += 0.001f;
    if (pulseCounter >= 2 * 3.14159f) pulseCounter -= 2 * 3.14159f;

    float pulseRadius = radius + radius/3.6f * (pow(std::sin(pulseCounter),2)+1);

    if (!ToDisplay(screenX, screenY, pulseRadius, viewRect)) return;

    sf::Color atmosphereColor = shape.getFillColor();
    atmosphereColor.a = 100;

    sf::CircleShape atmosphere(pulseRadius);
    atmosphere.setOrigin({pulseRadius, pulseRadius});
    atmosphere.setPosition({screenX, screenY});
    atmosphere.setFillColor(atmosphereColor);
    atmosphere.setPointCount(100);

    window.draw(atmosphere);
}