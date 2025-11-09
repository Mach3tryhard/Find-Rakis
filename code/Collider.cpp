#include "Collider.h"
#include "Physics.h"

bool Collider::isCollidingWith(const Physics& thisf, const Physics& thatf,Collider& other) {
    Pair shipPos = thisf.getPosition();
    Pair thatPos = thatf.getPosition();
    double distance = sqrt(pow(shipPos.x-thatPos.x,2)+pow(shipPos.y-thatPos.y,2));
    return distance < (other.getRadius()+this->radius);
}
double Collider::getRadius() {
    return radius;
}

Physics Collider::resolveCollision(const Physics& shipPhys, const Physics& planetPhys, double planetRadius) const {
    Pair shipPos = shipPhys.getPosition();
    Pair planetPos = planetPhys.getPosition();
    Pair vel = shipPhys.getVelocity();

    double dx = shipPos.x - planetPos.x;
    double dy = shipPos.y - planetPos.y;
    double dist = std::sqrt(dx * dx + dy * dy);
    if (dist == 0) return shipPhys; // safety

    Pair normal = {dx / dist, dy / dist};
    double overlap = (radius + planetRadius) - dist;

    Physics result = shipPhys;

    // repara pozitia
    if (overlap > 0) {
        result.setPosition({
            shipPos.x + normal.x * overlap,
            shipPos.y + normal.y * overlap
        });
    }

    // sterge velocity de la planeta
    double dot = vel.x * normal.x + vel.y * normal.y;
    if (dot < 0) {
        vel.x -= dot * normal.x;
        vel.y -= dot * normal.y;
    }
    // damping
    vel.x *= 0;
    vel.y *= 0;
    result.setVelocity(vel);
    return result;
}
/*void Collider::drawDebug(sf::RenderWindow& window, const Physics& physics) {
    Pair pos = physics.getPosition();
    debugShape.setPosition({(float)pos.x, (float)pos.y});
    window.draw(debugShape);
}*/