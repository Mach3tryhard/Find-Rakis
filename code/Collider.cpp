#include "Collider.h"
#include "Physics.h"

bool Collider::isCollidingWith(Physics& thisf,Physics& thatf,Collider& other) {
    Pair shipPos = thisf.getPosition();
    Pair thatPos = thatf.getPosition();
    double distance = sqrt(pow(shipPos.x-thatPos.x,2)+pow(shipPos.y-thatPos.y,2));
    return distance < (other.getRadius()+this->radius);
}
double Collider::getRadius() {
    return radius;
}

Physics Collider::resolveCollision(const Physics& shipPhys, const Physics& planetPhys) {
    Pair shipPos = shipPhys.getPosition();
    Pair planetPos = planetPhys.getPosition();
    Pair vel = shipPhys.getVelocity();

    double dx = shipPos.x - planetPos.x;
    double dy = shipPos.y - planetPos.y;
    double dist = std::sqrt(dx*dx + dy*dy);
    if (dist == 0) return shipPhys;

    Pair normal = {dx / dist, dy / dist};
    double dot = vel.x * normal.x + vel.y * normal.y;
    if (dot < 0) {
        Pair newVel = {vel.x-dot*normal.x,vel.y-dot*normal.y };
        Physics newPhys = shipPhys;
        newPhys.setVelocity(newVel);
        return newPhys;
    }

    return shipPhys;
}
void Collider::drawDebug(sf::RenderWindow& window, const Physics& physics) {
    Pair pos = physics.getPosition();
    debugShape.setPosition({(float)pos.x, (float)pos.y});
    window.draw(debugShape);
}