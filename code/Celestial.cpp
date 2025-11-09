#include "Celestial.h"
#include "Physics.h"
#include "Pair.h"
#include <SFML/Graphics.hpp>
#include "SpaceShip.h"

void Celestial::Display(Pair player,sf::RenderWindow& window,sf::FloatRect& viewRect) {
    float radius = getRadius();
    Pair starPos = physics.getPosition();
    const sf::Vector2f screenCenter = {static_cast<float>(window.getSize().x/2), static_cast<float>(window.getSize().y/2)};
    float screenX = screenCenter.x + static_cast<float>(starPos.x - player.x);
    float screenY = screenCenter.y + static_cast<float>(starPos.y - player.y);
    if (viewRect.contains({screenX - radius, screenY - radius})
        || viewRect.contains({screenX + radius, screenY + radius})
        || viewRect.contains({screenX - radius, screenY + radius})
        || viewRect.contains({screenX + radius, screenY - radius}))
    {
        shape.setPosition({screenX, screenY});
        window.draw(shape);
    }
}
sf::CircleShape& Celestial::getShape() {
    return shape;
}

Physics& Celestial::getPhysics() {
    return physics;
}
double Celestial::getRadius() {
    return this->collider.getRadius();
}
Collider &Celestial::getCollider() {
    return collider;
}
std::ostream& operator<<(std::ostream& out,const Celestial& body){
    out<<"BODY\n";
    out<<body.physics<<'\n';
    out<<"Stats:\n";
    out<<"Index"<<body.index<<'\n'<<"Health:"<<body.health<<'\n';
    out<<"Color:"<<body.color<<'\n'<<"Solid:"<<body.solid<<'\n';
    return out;
}