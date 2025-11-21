#include "Blackhole.h"

const std::vector<sf::Color> Blackhole::possibleColors= {sf::Color(0x000000FF)};

void Blackhole::initialize(std::mt19937& gen,float orbitRadius) {
    std::uniform_real_distribution<> distrib_radius(100.0, 150.0);
    std::uniform_real_distribution<> distrib_mass(5000.0, 7000.0);

    double radius = distrib_radius(gen);
    this->getPhysics().setMass(distrib_mass(gen));
    this->collider = Collider(radius);
    this->orbitRadius = orbitRadius;
    health = 20000;

    shape.setRadius(radius);
    shape.setOrigin({static_cast<float>(radius), static_cast<float>(radius)});
    shape.setFillColor(possibleColors[0]);
    Pair pos = physics.getPosition();
    shape.setPosition({static_cast<float>(pos.x), static_cast<float>(pos.y)});
}
void Blackhole::CelestialEffects(sf::RenderWindow& window, Pair player, sf::FloatRect& viewRect) {
    Pair pos = physics.getPosition();
    const sf::Vector2f screenCenter = { window.getSize().x / 2.f, window.getSize().y / 2.f };
    float screenX = screenCenter.x + static_cast<float>(pos.x - player.x);
    float screenY = screenCenter.y + static_cast<float>(pos.y - player.y);

    float radius = getRadius();

    if (!ToDisplay(screenX, screenY, radius*3.f, viewRect)) return;

    rotationAngle += 0.01f;
    if (rotationAngle >= 360.f) rotationAngle -= 360.f;

    sf::CircleShape disk(radius * 1.5f);
    disk.setOrigin({ radius * 1.5f, radius * 1.5f });
    disk.setPosition({screenX, screenY});

    disk.setFillColor(sf::Color(254, 159, 35, 255));

    disk.setScale({ 2.0f, 0.9f });
    disk.setRotation(sf::degrees(rotationAngle));

    window.draw(disk);

    sf::CircleShape whitecircle(radius * 1.25f);
    whitecircle.setOrigin({ radius * 1.25f, radius * 1.25f});
    whitecircle.setPosition({screenX, screenY});

    whitecircle.setFillColor(sf::Color(253, 240, 180, 200));

    whitecircle.setScale({ 1.f, 1.f });

    window.draw(whitecircle);


    sf::CircleShape inner(radius);
    inner.setOrigin({ radius, radius});
    inner.setPosition({screenX, screenY});

    inner.setFillColor(sf::Color(0, 0, 0, 255));

    inner.setScale({ 1.f, 1.f });

    window.draw(inner);

    float pulseSpeed = 0.1f;
    pulseTimer += pulseSpeed;
    if (pulseTimer>radius)
        pulseTimer = 0;
    float currentPulseRadius = pulseTimer;

    sf::CircleShape pulse(currentPulseRadius);
    pulse.setOrigin({ currentPulseRadius, currentPulseRadius });
    pulse.setPosition({ screenX, screenY });
    pulse.setFillColor(sf::Color(255, 255, 255, 15));

    window.draw(pulse);
}