#include <SFML/Graphics.hpp>
#ifndef OOP_GUI_H
#define OOP_GUI_H
#include "SpaceShip.h"
#include "SolarSystem.h"

class GUI {
private:
    sf::Font font;
    sf::Text debugText;
public:
    GUI() : debugText(font) {
        this->font = sf::Font("fonts/jetbrains.ttf");
        this->debugText = sf::Text(font);
        this->debugText.setFont(font);
        this->debugText.setCharacterSize(18);
        this->debugText.setFillColor(sf::Color::White);
    }
    sf::Text& getText();
    void UpdateGUI(SpaceShip& player,SolarSystem solar_system);
};

#endif //OOP_GUI_H