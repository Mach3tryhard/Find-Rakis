#include "Universe.h"
#include <functional>
#include "SpaceShip.h"

void Universe::Display(Pair p, sf::RenderWindow& w, sf::FloatRect& v, sf::Texture& t) {
    for (int i = 0; i < systems.size(); i++) {
        systems[i].Display(p, w, v, t);
    }
}

void Universe::computeGravity(SpaceShip& player) {
    for (int i = 0; i < systems.size(); i++) {
        systems[i].computeGravity(player);
    }
}

std::vector<SolarSystem>& Universe::getSystems() {
    return systems;
}
std::ostream& operator<<(std::ostream& out,const Universe& universe) {
    out<<"THE WHOLE UNIVERSE : \n";
    for (const auto& each_system : universe.systems) {
        out<<each_system<<'\n';
    }
    return out;
}