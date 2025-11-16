#include "Planet.h"

const std::vector<sf::Color> Planet::possibleColors= {sf::Color::Blue,sf::Color::Green,sf::Color::Red};

void Planet::initialize(std::mt19937& gen) {
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
void Planet::CelestialEffects(sf::RenderWindow& window, Pair player, sf::FloatRect& viewRect) {
    float r = getRadius();

    pulseCounter += 0.0005f;
    if (pulseCounter >= 2 * 3.14159f) pulseCounter -= 2 * 3.14159f;

    float pulseRadius = r + r/6 * (pow(std::sin(pulseCounter),2)+1);

    Pair pos = physics.getPosition();
    const sf::Vector2f screenCenter = { window.getSize().x / 2.f, window.getSize().y / 2.f };
    float screenX = screenCenter.x + static_cast<float>(pos.x - player.x);
    float screenY = screenCenter.y + static_cast<float>(pos.y - player.y);

    if (!ToDisplay(screenX, screenY, pulseRadius, viewRect)) return;

    sf::Color planetColor = shape.getFillColor();
    sf::Color atmosphereColor = planetColor;
    atmosphereColor.a = 100;

    sf::CircleShape atmosphere(pulseRadius);
    atmosphere.setOrigin({pulseRadius, pulseRadius});
    atmosphere.setPosition({screenX, screenY});
    atmosphere.setFillColor(atmosphereColor);

    window.draw(atmosphere);
}