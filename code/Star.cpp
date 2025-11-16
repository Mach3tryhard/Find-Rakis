#include "Star.h"

const std::vector<sf::Color> Star::possibleColors= {sf::Color::White,sf::Color::White,sf::Color::White};

void Star::initialize(std::mt19937& gen) {
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
void Star::CelestialEffects(sf::RenderWindow& window, Pair player, sf::FloatRect& viewRect) {
    float r = getRadius();

    Pair pos = physics.getPosition();
    const sf::Vector2f screenCenter = { window.getSize().x / 2.f, window.getSize().y / 2.f };
    float screenX = screenCenter.x + static_cast<float>(pos.x - player.x);
    float screenY = screenCenter.y + static_cast<float>(pos.y - player.y);

    if (!ToDisplay(screenX, screenY, r*1.4f, viewRect)) return;

    static float rotationOffset = 0.f;
    rotationOffset += 0.01f;
    if (rotationOffset >= 360.f) rotationOffset -= 360.f;

    float triSize = r * 1.4f;

    sf::CircleShape triangle1(triSize, 3);
    triangle1.setOrigin({triSize, triSize});
    triangle1.setPosition({screenX, screenY});
    triangle1.setRotation(sf::degrees(rotationOffset));
    triangle1.setFillColor(sf::Color(255, 255, 255, 100));

    sf::CircleShape triangle2(triSize, 3);
    triangle2.setOrigin({triSize, triSize});
    triangle2.setPosition({screenX, screenY});
    triangle2.setRotation(sf::degrees(rotationOffset + 180.f));
    triangle2.setFillColor(sf::Color(255, 255, 255, 100));

    sf::CircleShape triangle3(triSize, 3);
    triangle3.setOrigin({triSize, triSize});
    triangle3.setPosition({screenX, screenY});
    triangle3.setRotation(sf::degrees(rotationOffset + 90.f));
    triangle3.setFillColor(sf::Color(255, 255, 255, 100));

    sf::CircleShape triangle4(triSize, 3);
    triangle4.setOrigin({triSize, triSize});
    triangle4.setPosition({screenX, screenY});
    triangle4.setRotation(sf::degrees(rotationOffset + 270.f));
    triangle4.setFillColor(sf::Color(255, 255, 255, 100));

    window.draw(triangle1);
    window.draw(triangle2);
    window.draw(triangle3);
    window.draw(triangle4);
}