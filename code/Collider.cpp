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

Physics Collider::resolveCollision(const Physics& shipPhys,const Physics& planetPhys,double planetRadius) const{
    Pair shipPos = shipPhys.getPosition();
    Pair planetPos = planetPhys.getPosition();
    Pair shipVel = shipPhys.getVelocity();
    Pair planetVel = planetPhys.getVelocity();

    double dx = shipPos.x - planetPos.x;
    double dy = shipPos.y - planetPos.y;
    double dist = std::sqrt(dx*dx + dy*dy);

    double target = radius + planetRadius;

    Physics result = shipPhys;

    if (dist == 0) {
        result.setPosition({ planetPos.x + target, planetPos.y });
        result.setVelocity(planetVel);
        return result;
    }

    Pair n = { dx / dist, dy / dist };

    if (dist < target) {
        double overlap = target - dist;

        double push = overlap * 0.8;
        result.setPosition({
            shipPos.x + n.x * push,
            shipPos.y + n.y * push
        });
    }

    Pair rel = { shipVel.x - planetVel.x, shipVel.y - planetVel.y };
    double relNormal = rel.x * n.x + rel.y * n.y;
    Pair newRelVel = rel;
    if (relNormal < 0.0) {
        newRelVel.x -= relNormal * n.x;
        newRelVel.y -= relNormal * n.y;
    }

    const double damping = 0.20;
    newRelVel.x *= (1.0 - damping);
    newRelVel.y *= (1.0 - damping);

    result.setVelocity({
        planetVel.x + newRelVel.x,
        planetVel.y + newRelVel.y
    });
    return result;
}
