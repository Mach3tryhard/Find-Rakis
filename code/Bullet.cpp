#include "Bullet.h"
#include "Pair.h"
#include <SFML/Graphics.hpp>
#include "Physics.h"

void Bullet::Display(const Pair& position, sf::RenderWindow& window, sf::FloatRect& viewRect) {
    Pair shipPos = position;
    Pair bulletPos = physics.getPosition();
    const sf::Vector2f screenCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);
    float screenX = screenCenter.x + static_cast<float>(bulletPos.x - shipPos.x);
    float screenY = screenCenter.y + static_cast<float>(bulletPos.y - shipPos.y);

    if (viewRect.contains({screenX, screenY})) {
        shape.setPosition({screenX, screenY});
        window.draw(shape);
    }
}
void Bullet::Update(sf::Time dt,Pair player,sf::RenderWindow& window,sf::FloatRect& viewRect) {
    lifetime-=dt.asSeconds();
    physics.UpdatePosition(dt);
    Display(player,window,viewRect);
}
sf::CircleShape& Bullet::getShape() {
    return shape;
}
Physics& Bullet::getPhysics() {
    return physics;
}
float Bullet::getDamage() const {
    return damage;
}
float Bullet::getLifetime() const {
    return lifetime;
}

Collider &Bullet::getCollider(){
    return collider;
}
const float Bullet::speed = 200.f;
Bullet &Bullet::operator=(const Bullet &other) {
    if (this!=&other) {
        shape = other.shape;
        physics = other.physics;
        collider = other.collider;
        damage = other.damage;
        radius = other.radius;
        lifetime = other.lifetime;
    }
    return *this;
}
std::ostream& operator<<(std::ostream& out,const Bullet& bullet) {
    out<<"BULLET\n";
    out<<bullet.physics<<'\n';
    out<<"Stats:\n";
    out<<"Damage:"<<bullet.damage<<'\n';
    return out;
}