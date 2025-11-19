#include "Star.h"

const std::vector<sf::Color> Star::possibleColors= {sf::Color(0X92b5ffFF),sf::Color(0Xa2c0ffFF),sf::Color(0Xd5e0ffFF),sf::Color(0Xffede3FF),sf::Color(0Xffdab5FF),sf::Color(0Xffb56cFF),sf::Color(0Xf9f5ffFF)};

void Star::initialize(std::mt19937& gen) {
    std::uniform_real_distribution<> distrib_radius(200.0, 250.0);
    std::uniform_real_distribution<> distrib_mass(5000.0, 6000.0);
    std::uniform_int_distribution<> distrib_color(0, 6);

    double radius = distrib_radius(gen);
    this->getPhysics().setMass(distrib_mass(gen));
    this->collider = Collider(radius);
    health = 500;

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

    sf::Color starcolor= shape.getFillColor();
    starcolor.a = 100;

    sf::CircleShape triangle1(triSize, 3);
    triangle1.setOrigin({triSize, triSize});
    triangle1.setPosition({screenX, screenY});
    triangle1.setRotation(sf::degrees(rotationOffset));
    triangle1.setFillColor(starcolor);

    sf::CircleShape triangle2(triSize, 3);
    triangle2.setOrigin({triSize, triSize});
    triangle2.setPosition({screenX, screenY});
    triangle2.setRotation(sf::degrees(rotationOffset + 180.f));
    triangle2.setFillColor(starcolor);

    sf::CircleShape triangle3(triSize, 3);
    triangle3.setOrigin({triSize, triSize});
    triangle3.setPosition({screenX, screenY});
    triangle3.setRotation(sf::degrees(rotationOffset + 90.f));
    triangle3.setFillColor(starcolor);

    sf::CircleShape triangle4(triSize, 3);
    triangle4.setOrigin({triSize, triSize});
    triangle4.setPosition({screenX, screenY});
    triangle4.setRotation(sf::degrees(rotationOffset + 270.f));
    triangle4.setFillColor(starcolor);

    window.draw(triangle1);
    window.draw(triangle2);
    window.draw(triangle3);
    window.draw(triangle4);
}
void Star::RefuelCheck(SpaceShip& player) {
    double dist = sqrtf(pow(physics.getPosition().x-player.getPhysics().getPosition().x,2)+pow(physics.getPosition().y-player.getPhysics().getPosition().y,2));
    if (dist<150.f + getRadius()) {
        player.SetRefuel(true,physics.getPosition(),getRadius());
    }
}