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
}

void GUI::UpdateGUI(SpaceShip& player,Celestial solar_system) {
    std::string posText ="position: x: " + std::to_string(player.getPhysics().getPosition().x)
    +" y: " + std::to_string(player.getPhysics().getPosition().y)
    + "\ndirection: "+ std::to_string(player.getShape().getRotation().asDegrees())
    + "\nvelocity: x " + std::to_string(player.getPhysics().getVelocity().x)
    +"y: " + std::to_string(player.getPhysics().getVelocity().y)
    + "\nacceleration: x " + std::to_string(player.getPhysics().getAcceleration().x)
    +" y: " + std::to_string(player.getPhysics().getAcceleration().y)
    +"\nSolar System-> x: " + std::to_string(solar_system.getPhysics().getPosition().x)
    +" y: " + std::to_string(solar_system.getPhysics().getPosition().y);
    debugText.setString(posText);
}

void GUI::DrawMiniMap(sf::RenderWindow& window, Universe& universe, SpaceShip& player) {
    window.setView(minimapView);

    // Draw solar system centers
    for (auto& system : universe.getSystems()) {
        sf::CircleShape dot(300.f); // world units
        dot.setOrigin({150.f, 150.f}); // center the dot
        dot.setFillColor(sf::Color::White);
        auto pos = system.getPhysics().getPosition();
        dot.setPosition({static_cast<float>(pos.x), static_cast<float>(pos.y)});
        window.draw(dot);
    }

    // Draw player
    sf::CircleShape playerDot(200.f);
    playerDot.setOrigin({100.f, 100.f});
    playerDot.setFillColor(sf::Color::Cyan);
    auto playerPos = player.getPhysics().getPosition();
    playerDot.setPosition({static_cast<float>(playerPos.x), static_cast<float>(playerPos.y)});
    window.draw(playerDot);

    // Reset to default view
    window.setView(window.getDefaultView());
    window.draw(minimapBox); // draw border
}