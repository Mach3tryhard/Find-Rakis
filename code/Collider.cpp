#include "Collider.h"
#include "Physics.h"

bool Collider::isCollidingWith(Physics thisf,Physics thatf,Collider collider) {
    Pair shipPos = thisf.getPosition();
    Pair thatPos = thatf.getPosition();
    double distance = sqrt(pow(shipPos.x-thatPos.x,2)+pow(shipPos.y-thatPos.y,2));
    return distance < collider.getRadius();
}
double Collider::getRadius() {
    return radius;
}
Physics addNormalVector(Physics ship) {
    Pair currentAcceleration=ship.getAcceleration();
    ship.addAcceleration({-currentAcceleration.x,-currentAcceleration.y});
    return ship;
}