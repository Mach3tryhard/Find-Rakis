#include "GUI.h"
#include "Bullet.h"
#include "Pair.h"
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Physics.h"
#include "SpaceShip.h"
#include "SolarSystem.h"

void GUI::Initialize(sf::Window &window) {
    float left = 10.f / window.getSize().x;
    float bottom = 10.f / window.getSize().y;
    float width = minimapSize / window.getSize().x;
    float height = minimapSize / window.getSize().y;
    minimapView.setViewport(sf::FloatRect({left, 1.f - height - bottom}, {width, height}));
    minimapBox.setPosition({10.f, window.getSize().y - minimapSize-10.f});

    debugText.setPosition({static_cast<float>(window.getSize().x*2/3)-175.f,window.getSize().y -100.f});
    barsText.setPosition({static_cast<float>(window.getSize().x/3)-300.f,window.getSize().y -75.f});

    dataText.setPosition({(window.getSize().x/3-50.f),window.getSize().y -100.f});

    FuelBar.setPosition({static_cast<float>(window.getSize().x/3)-200.f,window.getSize().y -63.f});
    EnergyBar.setPosition({static_cast<float>(window.getSize().x/3)-200.f,window.getSize().y -37.f});
    OreBar.setPosition({static_cast<float>(window.getSize().x/3)-200.f,window.getSize().y -10.f});
    FuelBarback.setPosition({static_cast<float>(window.getSize().x/3)-200.f,window.getSize().y -63.f});
    EnergyBarback.setPosition({static_cast<float>(window.getSize().x/3)-200.f,window.getSize().y -37.f});
    OreBarback.setPosition({static_cast<float>(window.getSize().x/3)-200.f,window.getSize().y -10.f});

    float right = 10.f / window.getSize().x;
    arrowView.setViewport(sf::FloatRect({1.f - width - right, 1.f - height - bottom},{width, height}));
    arrowView.setCenter({0.f, 0.f});
    arrowView.setSize({minimapSize, minimapSize});
    arrowbox.setPosition({window.getSize().x - minimapSize-10.f, window.getSize().y - minimapSize-10.f});
}

void GUI::DrawText(sf::RenderWindow& window,SpaceShip& player){
    std::string posText ="position:     x: " + std::to_string(player.getPhysics().getPosition().x)
    +" y: " + std::to_string(player.getPhysics().getPosition().y)
    + "\nvelocity:     x: " + std::to_string(player.getPhysics().getVelocity().x)
    +" y: " + std::to_string(player.getPhysics().getVelocity().y)
    + "\nacceleration: x: " + std::to_string(player.getPhysics().getAcceleration().x)
    +" y: " + std::to_string(player.getPhysics().getAcceleration().y)
    + "\ndirection:    d: "+ std::to_string(player.getShape().getRotation().asDegrees());
    debugText.setString(posText);
    window.draw(debugText);

    std::string datatext="hours:   "+std::to_string(static_cast<int>( player.getTimer())/3600)+
    "\nminutes: "+std::to_string(static_cast<int>(player.getTimer())/60%60)+
    "\nseconds: "+std::to_string(static_cast<int>(player.getTimer())%60)+
    "\ndistance:"+std::to_string(player.getDistance_travelled());
    dataText.setString(datatext);

    window.draw(dataText);
}

sf::Vector2f GUI::WorldToMiniMap(Pair worldPos, int constraint, sf::Vector2f minimapSize)
{
    float nx = (worldPos.x + constraint) / (2.f * constraint);
    float ny = (worldPos.y + constraint) / (2.f * constraint);
    return { nx * minimapSize.x, ny * minimapSize.y };
}

void GUI::DrawMiniMap(sf::RenderWindow& window, Universe& universe, SpaceShip& player)
{
    sf::RectangleShape minimapBackground(minimapBox.getSize());
    minimapBackground.setPosition(minimapBox.getPosition());
    minimapBackground.setFillColor(sf::Color(20, 20, 20, 255));
    window.draw(minimapBackground);

    window.setView(minimapView);

    for (auto& system : universe.getSystems()) {
        for (auto& body: system.getBodies()) {
            sf::CircleShape dot1(200.f);
            dot1.setOrigin({100.f, 100.f});
            dot1.setFillColor(sf::Color::White);
            auto pos = body->getPhysics().getPosition();
            dot1.setPosition({static_cast<float>(pos.x), static_cast<float>(pos.y)});
            window.draw(dot1);
        }
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

    auto p = player.getPhysics().getPosition();
    playerDot.setPosition({static_cast<float>(p.x), static_cast<float>(p.y)});
    window.draw(playerDot);

    // Restore normal view
    window.setView(window.getDefaultView());

    // Draw minimap frame overlay
    window.draw(minimapBox);
}



/*void GUI::DrawMiniMap(sf::RenderWindow& window, Universe& universe, SpaceShip& player) {
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
}*/

void GUI::DrawBars(sf::RenderWindow& window,const SpaceShip& player) {
    FuelBar.setSize({player.getFuel(),10.f});
    EnergyBar.setSize({player.getEnergy(),10.f});
    OreBar.setSize({player.getOre(),10.f});
    FuelBar.setOrigin({0.f, 5.f});
    EnergyBar.setOrigin({0.f, 5.f});
    OreBar.setOrigin({0.f, 5.f});

    FuelBarback.setOrigin({0.f, 5.f});
    EnergyBarback.setOrigin({0.f, 5.f});
    OreBarback.setOrigin({0.f, 5.f});

    std::string text ="fuel:  \nenergy:\nore:    ";
    barsText.setString(text);
    window.draw(barsText);

    window.draw(FuelBarback);
    window.draw(EnergyBarback);
    window.draw(OreBarback);
    window.draw(FuelBar);
    window.draw(EnergyBar);
    window.draw(OreBar);
}

void GUI::DrawArrowHUD(sf::RenderWindow& window,SpaceShip& player) {
    sf::RectangleShape arrowBackground(arrowbox.getSize());
    arrowBackground.setPosition(arrowbox.getPosition());
    arrowBackground.setFillColor(sf::Color(20, 20, 20, 255));
    window.draw(arrowBackground);

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