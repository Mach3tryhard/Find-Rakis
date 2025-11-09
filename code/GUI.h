#pragma once
#include <SFML/Graphics.hpp>
#ifndef OOP_GUI_H
#define OOP_GUI_H
#include "SpaceShip.h"
#include "SolarSystem.h"
#include "Universe.h"

class GUI {
private:
    sf::Font font;
    sf::Text debugText;

    sf::View minimapView;
    sf::RectangleShape minimapBox;
    float minimapSize = 300.f;

    sf::RectangleShape velocityLine;
    sf::ConvexShape velocityTip;
    sf::RectangleShape arrowbox;
    sf::View arrowView;
public:
    GUI() : debugText(font) {
        this->font = sf::Font("fonts/jetbrains.ttf");
        this->debugText = sf::Text(font);
        this->debugText.setFont(font);
        this->debugText.setCharacterSize(18);
        this->debugText.setFillColor(sf::Color::White);

        minimapView = sf::View(sf::Vector2f(0.f, 0.f), sf::Vector2f(21600.f, 21600.f));
        minimapBox.setSize({minimapSize, minimapSize});
        minimapBox.setFillColor(sf::Color(0,0,0,10));
        minimapBox.setOutlineColor(sf::Color::White);
        minimapBox.setOutlineThickness(2.f);

        velocityLine.setSize({1.f, 3.f});
        velocityLine.setFillColor(sf::Color::White);
        velocityLine.setOrigin({0.f, 1.5f});
        velocityTip.setPointCount(3);
        velocityTip.setPoint(0, sf::Vector2f(0.f, 0.f));
        velocityTip.setPoint(1, sf::Vector2f(-15.f, 7.5f));
        velocityTip.setPoint(2, sf::Vector2f(-15.f, -7.5f));
        velocityTip.setFillColor(sf::Color::White);
        arrowbox.setSize({minimapSize, minimapSize});
        arrowbox.setFillColor(sf::Color(0,0,0,10));
        arrowbox.setOutlineColor(sf::Color::White);
        arrowbox.setOutlineThickness(2.f);
    }
    void Initialize(sf::Window &window);
    sf::Text& getText();
    void DrawText(SpaceShip& player);
    void DrawMiniMap(sf::RenderWindow& window,Universe& universe,SpaceShip& player);
    void DrawVelocityArrowHUD(sf::RenderWindow& window,SpaceShip& player);
};

#endif //OOP_GUI_H