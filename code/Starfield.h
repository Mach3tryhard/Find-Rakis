#ifndef OOP_STARFIELD_H
#define OOP_STARFIELD_H
#include <vector>

#include <SFML/Graphics.hpp>
#include "SpaceShip.h"

class Starfield {
private:
    struct StarDot {
        sf::Vector2f pos;
        float depth;
        sf::Color color;
    };
    std::vector<StarDot> stars;
    sf::Vector2u screen;
    float speed = 80.f;
public:
    static const std::vector<sf::Color> possibleColors;
    Starfield(unsigned count, sf::Vector2u screenSize) : screen(screenSize) {
        stars.reserve(count);
        for (unsigned i = 0; i < count; i++) {
            StarDot s;
            s.pos = { float(rand() % screen.x), float(rand() % screen.y) };
            s.depth = 0.3f + (rand() % 70) / 100.f;
            int c = 150 + int(105 * s.depth);
            s.color = possibleColors[rand() % possibleColors.size()];
            s.color.a=c;
            //s.color = sf::Color(c, c, c,c);
            stars.push_back(s);
        }
    }
    void update(sf::Vector2f playerVel, float dt, const SpaceShip& player);
    void draw(sf::RenderWindow& win);
    void Update(sf::Time dt,sf::RenderWindow& window,SpaceShip& player);
};



#endif //OOP_STARFIELD_H