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