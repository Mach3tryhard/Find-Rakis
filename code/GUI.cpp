#include "GUI.h"
#include "Bullet.h"
#include "Pair.h"
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Physics.h"
#include "SpaceShip.h"
#include "SolarSystem.h"

sf::Text& GUI::getText() {
    return this->debugText;
}

void GUI::Initialize(sf::Window &window) {
    float left = 10.f / window.getSize().x;
    float bottom = 10.f / window.getSize().y;
    float width = minimapSize / window.getSize().x;
    float height = minimapSize / window.getSize().y;
    minimapView.setViewport(sf::FloatRect({left, 1.f - height - bottom}, {width, height}));
    minimapBox.setPosition({10.f, window.getSize().y - minimapSize-10.f});

    debugText.setPosition({static_cast<float>(window.getSize().x/2)-200.f,window.getSize().y -100.f});

    float right = 10.f / window.getSize().x;
    arrowView.setViewport(sf::FloatRect({1.f - width - right, 1.f - height - bottom},{width, height}));
    arrowView.setCenter({0.f, 0.f});
    arrowView.setSize({minimapSize, minimapSize});
    arrowbox.setPosition({window.getSize().x - minimapSize-10.f, window.getSize().y - minimapSize-10.f});
}

void GUI::DrawText(SpaceShip& player) {
    std::string posText ="position:     x: " + std::to_string(player.getPhysics().getPosition().x)
    +" y: " + std::to_string(player.getPhysics().getPosition().y)
    + "\ndirection:    d: "+ std::to_string(player.getShape().getRotation().asDegrees())
    + "\nvelocity:     x: " + std::to_string(player.getPhysics().getVelocity().x)
    +" y: " + std::to_string(player.getPhysics().getVelocity().y)
    + "\nacceleration: x: " + std::to_string(player.getPhysics().getAcceleration().x)
    +" y: " + std::to_string(player.getPhysics().getAcceleration().y);
    debugText.setString(posText);
}

void GUI::DrawMiniMap(sf::RenderWindow& window, Universe& universe, SpaceShip& player) {
    window.setView(minimapView);

    for (auto& system : universe.getSystems()) {
        sf::CircleShape dot(300.f);
        dot.setOrigin({150.f, 150.f});
        dot.setFillColor(sf::Color::Yellow);
        auto pos = system.getPhysics().getPosition();
        dot.setPosition({static_cast<float>(pos.x), static_cast<float>(pos.y)});
        window.draw(dot);
    }

    sf::CircleShape playerDot(200.f);
    playerDot.setOrigin({100.f, 100.f});
    playerDot.setFillColor(sf::Color::Cyan);
    auto playerPos = player.getPhysics().getPosition();
    playerDot.setPosition({static_cast<float>(playerPos.x), static_cast<float>(playerPos.y)});
    window.draw(playerDot);

    window.setView(window.getDefaultView());
    window.draw(minimapBox);
}

void GUI::DrawBars(sf::RenderWindow& window, const SpaceShip& player) {
    FuelBar.setSize({player.getFuel(),10.f});
    EnergyBar.setSize({player.getEnergy(),10.f});
    OreBar.setSize({player.getOre(),10.f});
    FuelBar.setOrigin({0.f, 5.f});
    EnergyBar.setOrigin({0.f, 5.f});
    OreBar.setOrigin({0.f, 5.f});

    float sizex=window.getSize().x;
    float sizey=window.getSize().y;

    FuelBar.setPosition({sizex/3, sizey-125.f});
    EnergyBar.setPosition({sizex/3, sizey-100.f});
    OreBar.setPosition({sizex/3, sizey-75.f});

    FuelBarback.setOrigin({0.f, 5.f});
    EnergyBarback.setOrigin({0.f, 5.f});
    OreBarback.setOrigin({0.f, 5.f});

    FuelBarback.setPosition({sizex/3, sizey-125.f});
    EnergyBarback.setPosition({sizex/3, sizey-100.f});
    OreBarback.setPosition({sizex/3, sizey-75.f});

    window.draw(FuelBarback);
    window.draw(EnergyBarback);
    window.draw(OreBarback);
    window.draw(FuelBar);
    window.draw(EnergyBar);
    window.draw(OreBar);
}

void GUI::DrawArrowHUD(sf::RenderWindow& window,SpaceShip& player) {
    window.setView(arrowView);

    sf::Vector2f velocity = {static_cast<float>(player.getPhysics().getVelocity().x), static_cast<float>(player.getPhysics().getVelocity().y)};
    float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (length < 1.f) length = 1.f;
    float angleRad = std::atan2(velocity.y, velocity.x);
    float maxLength = minimapSize * 0.4f;
    float scaledLength = std::min(length * 0.5f, maxLength);

    sf::Vector2f center(0.f, 0.f);
    velocityLine.setSize({scaledLength, 5.f});
    velocityLine.setOrigin({0.f, 2.5f});
    velocityLine.setPosition(center);
    velocityLine.setRotation(sf::radians(angleRad));
    velocityTip.setRotation(sf::radians(angleRad));
    velocityTip.setPosition(center + sf::Vector2f(std::cos(angleRad) * (scaledLength+10.f),std::sin(angleRad) * (scaledLength+10.f)));

    float normalsize=30.f;
    float directionAngleRad = player.getShape().getRotation().asRadians()-90.f*3.14159265f/180.f;
    directionLine.setSize({normalsize, 5.f});
    directionLine.setOrigin({0.f, 2.5f});
    directionLine.setPosition(center);
    directionLine.setRotation(sf::radians(directionAngleRad));
    directionTip.setRotation(sf::radians(directionAngleRad));
    directionTip.setPosition(center + sf::Vector2f(std::cos(directionAngleRad) * (normalsize+10.f),std::sin(directionAngleRad) * (normalsize+10.f)));

    window.draw(velocityLine);
    window.draw(velocityTip);

    window.draw(directionLine);
    window.draw(directionTip);

    window.setView(window.getDefaultView());
    window.draw(arrowbox);
}