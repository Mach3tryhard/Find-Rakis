#include "Starfield.h"
#include "SpaceShip.h"

const std::vector<sf::Color> Starfield::possibleColors = {sf::Color::White,sf::Color(0XFAEDCBFF),sf::Color(0XC9E4DEFF),sf::Color(0XC6DEF1FF),sf::Color(0XDBCDF0FF),sf::Color(0XF2C6DEFF),sf::Color(0XF7D9C4FF)};

void Starfield::update(sf::Vector2f playerVel, float dt, const SpaceShip& player) {
    for (auto& s : stars) {
        if (player.getHyper())
            s.pos-=playerVel * s.depth * speed * dt/50.f;
        else
            s.pos -= playerVel * s.depth * speed * dt/150.f;

        if (s.pos.x < 0) s.pos.x += screen.x;
        if (s.pos.x >= screen.x) s.pos.x -= screen.x;
        if (s.pos.y < 0) s.pos.y += screen.y;
        if (s.pos.y >= screen.y) s.pos.y -= screen.y;
    }
}
void Starfield::draw(sf::RenderWindow& window) {
    sf::RectangleShape pix({2,2});
    for (auto& s : stars) {
        pix.setPosition(s.pos);
        pix.setFillColor(s.color);
        window.draw(pix);
    }
}
void Starfield::Update(sf::Time dt,sf::RenderWindow& window,SpaceShip& player) {
    sf::Vector2f vel = {
        (float)player.getPhysics().getVelocity().x,
        (float)player.getPhysics().getVelocity().y
    };

    update(vel, dt.asSeconds(),player);
    draw(window);
}