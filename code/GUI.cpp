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

void GUI::UpdateGUI(SpaceShip& player) {
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

void GUI::DrawVelocityArrowHUD(sf::RenderWindow& window,SpaceShip& player) {
    // Switch to arrow HUD view
    window.setView(arrowView);

    sf::Vector2f velocity = {static_cast<float>(player.getPhysics().getVelocity().x), static_cast<float>(player.getPhysics().getVelocity().y)};
    float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (length < 1.f) length = 1.f; // minimal visible length

    float angleRad = std::atan2(velocity.y, velocity.x);

    // Normalize scale so the arrow fits in the box
    float maxLength = minimapSize * 0.4f;
    float scaledLength = std::min(length * 0.5f, maxLength);

    // Draw from center of the box view (0,0 is center in this view)
    sf::Vector2f center(0.f, 0.f);
    velocityLine.setSize({scaledLength, 5.f});
    velocityLine.setOrigin({0.f, 2.5f});
    velocityLine.setPosition(center);
    velocityLine.setRotation(sf::radians(angleRad));

    velocityTip.setRotation(sf::radians(angleRad));
    velocityTip.setPosition(center + sf::Vector2f(std::cos(angleRad) * (scaledLength+10.f),
                                                  std::sin(angleRad) * (scaledLength+10.f)));

    // Draw arrow and reset to default view
    window.draw(velocityLine);
    window.draw(velocityTip);

    window.setView(window.getDefaultView());
    window.draw(arrowbox);
}