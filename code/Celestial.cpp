#include "Celestial.h"
#include "Physics.h"
#include "Pair.h"
#include <SFML/Graphics.hpp>
#include "SpaceShip.h"

Collider &Celestial::getCollider(){return collider;}
double Celestial::getRadius() {
    return this->collider.getRadius();
}
void Celestial::Display(SpaceShip& player,sf::RenderWindow& window,sf::FloatRect& viewRect) {
    float radius = getRadius();
    Pair shipPos = player.getPhysics().getPosition();
    Pair starPos = physics.getPosition();
    const sf::Vector2f screenCenter = {static_cast<float>(window.getSize().x/2), static_cast<float>(window.getSize().y/2)};
    float screenX = screenCenter.x + static_cast<float>(starPos.x - shipPos.x);
    float screenY = screenCenter.y + static_cast<float>(starPos.y - shipPos.y);
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
std::ostream& operator<<(std::ostream& out,const Celestial& body){
    out<<"BODY\n";
    out<<body.physics<<'\n';
    out<<"Stats:\n";
    out<<"Index"<<body.index<<'\n'<<"Health:"<<body.health<<'\n';
    out<<"Gravity:"<<body.gravity<<'\n';
    out<<"Color:"<<body.color<<'\n'<<"Solid:"<<body.solid<<'\n';
    return out;
}