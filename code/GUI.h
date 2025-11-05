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
public:
    GUI() : debugText(font) {
        this->font = sf::Font("fonts/jetbrains.ttf");
        this->debugText = sf::Text(font);
        this->debugText.setFont(font);
        this->debugText.setCharacterSize(18);
        this->debugText.setFillColor(sf::Color::White);

        float viewportWidth = 0.2f;
        float viewportHeight = 0.25f;
        float viewportAspect = viewportWidth / viewportHeight;
        float universeWidth = 21600.f;
        float universeHeight = 21600.f;

        if (universeWidth / universeHeight > viewportAspect) {
            float newHeight = universeWidth / viewportAspect;
            minimapView.setSize(sf::Vector2f(universeWidth, newHeight));
        } else {
            float newWidth = universeHeight * viewportAspect;
            minimapView.setSize(sf::Vector2f(newWidth, universeHeight));
        }
        minimapView = sf::View(sf::Vector2f(0.f, 0.f), sf::Vector2f(21600.f, 21600.f));

        minimapBox.setSize({minimapSize, minimapSize});
        minimapBox.setFillColor(sf::Color(0,0,0,150));
        minimapBox.setOutlineColor(sf::Color::White);
        minimapBox.setOutlineThickness(2.f);
    }
    void Initialize(sf::Window &window);
    sf::Text& getText();
    void UpdateGUI(SpaceShip& player,Celestial solar_system);
    void DrawMiniMap(sf::RenderWindow& window,Universe& universe,SpaceShip& player);
};

#endif //OOP_GUI_H