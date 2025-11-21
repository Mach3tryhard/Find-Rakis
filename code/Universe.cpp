#include "Universe.h"
#include <functional>
#include "SpaceShip.h"

void Universe::Update(SpaceShip& player,sf::RenderWindow& window,sf::FloatRect& viewRect,sf::Texture &texture,sf::Time dt) {
    for (long unsigned int i = 0; i < systems.size(); i++) {
        systems[i].Update(player,window,viewRect,texture,dt);
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