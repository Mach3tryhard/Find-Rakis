#include "Celestial.h"
#include "Physics.h"
#include "Pair.h"
#include <SFML/Graphics.hpp>
#include "SpaceShip.h"

void Celestial::Display(Pair player,sf::RenderWindow& window,sf::FloatRect& viewRect,sf::Texture &texture) {
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
        shape.setTexture(&texture);
        window.draw(shape);
    }
}
void Celestial::CheckHit(Bullet& bullet) {
    if (collider.isCollidingWith(bullet.getPhysics(),physics,collider)) {
        LoseHealth(bullet.getDamage());
    }
}
void Celestial::LoseHealth(float damage) {
    health-=damage;
    if (health<=0) {
        // KILL CELESTIAL
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
    out<<'\n'<<"Health:"<<body.health<<'\n';
    out<<"Color:"<<body.color<<'\n'<<"Solid:"<<body.solid<<'\n';
    return out;
}