#include "Planet.h"

const std::vector<sf::Color> Planet::possibleColors= {sf::Color(0Xc0a480FF),sf::Color(0Xc26d5cFF),sf::Color(0Xbfaf9bFF),sf::Color(0X426b8fFF),sf::Color(0Xf4dbc4FF),sf::Color(0Xd9b292FF),sf::Color(0X7bb368FF)};

void Planet::initialize(std::mt19937& gen) {
    std::uniform_real_distribution<> distrib_radius(100.0, 150.0);
    std::uniform_real_distribution<> distrib_mass(1000.0, 1500.0);
    std::uniform_int_distribution<> distrib_color(0, 6);

    double radius = distrib_radius(gen);
    this->getPhysics().setMass(distrib_mass(gen));
    this->collider = Collider(radius);
    health = 250;

    shape.setRadius(radius);
    shape.setOrigin({static_cast<float>(radius), static_cast<float>(radius)});
    shape.setFillColor(possibleColors[distrib_color(gen)]);
    Pair pos = physics.getPosition();
    shape.setPosition({static_cast<float>(pos.x), static_cast<float>(pos.y)});
}
void Planet::CelestialEffects(sf::RenderWindow& window, Pair player, sf::FloatRect& viewRect) {
    float r = getRadius();

    pulseCounter += 0.001f;
    if (pulseCounter >= 2 * 3.14159f) pulseCounter -= 2 * 3.14159f;

    float pulseRadius = r + r/3.6f * (pow(std::sin(pulseCounter),2)+1);

    Pair pos = physics.getPosition();
    const sf::Vector2f screenCenter = { window.getSize().x / 2.f, window.getSize().y / 2.f };
    float screenX = screenCenter.x + static_cast<float>(pos.x - player.x);
    float screenY = screenCenter.y + static_cast<float>(pos.y - player.y);

    if (!ToDisplay(screenX, screenY, pulseRadius, viewRect)) return;

    sf::Color atmosphereColor = shape.getFillColor();
    atmosphereColor.a = 100;

    sf::CircleShape atmosphere(pulseRadius);
    atmosphere.setOrigin({pulseRadius, pulseRadius});
    atmosphere.setPosition({screenX, screenY});
    atmosphere.setFillColor(atmosphereColor);

    window.draw(atmosphere);
}