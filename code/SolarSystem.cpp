#include "SolarSystem.h"
#include "Physics.h"
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Celestial.h"
#include "ParticleSystem.h"

void SolarSystem::Display(Pair player,sf::RenderWindow& window,sf::FloatRect& viewRect) {
    for (auto i:this->bodies) {
        i.Display(player,window,viewRect);
    }
}
Physics& SolarSystem::getPhysics() {
    return physics;
}
std::vector<Celestial>& SolarSystem::getBodies() {
    return bodies;
}

std::ostream& operator<<(std::ostream& out,const SolarSystem& system) {
    out<<"SOLAR SYSTEM\n";
    for (const auto& bod : system.bodies) {
        out<<bod<<'\n';
    }
    return out;
}