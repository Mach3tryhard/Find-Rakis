#include "Starfield.h"
#include "SpaceShip.h"

void Starfield::update(sf::Vector2f playerVel, float dt,SpaceShip& player) {
    for (auto& s : stars) {
        if (player.getHyper())
            s.pos-=playerVel * s.depth * speed * dt/75.f;
        else
            s.pos -= playerVel * s.depth * speed * dt/200.f;

        // wrap around screen
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