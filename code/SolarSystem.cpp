#include "SolarSystem.h"
#include "Physics.h"
#include <cmath>
#include <functional>
#include <SFML/Graphics.hpp>
#include "Celestial.h"
#include "ParticleSystem.h"
#include "SpaceShip.h"

void SolarSystem::Display(Pair player,sf::RenderWindow& window,sf::FloatRect& viewRect,sf::Texture &texture) {
    for (auto i:this->bodies) {
        i->Display(player,window,viewRect,texture);
    }
}

void SolarSystem::computeGravity(SpaceShip& player) {
    for (auto i:this->bodies) {
        i->computeGravity(player);
    }
}

Physics& SolarSystem::getPhysics() {
    return physics;
}
std::vector<Celestial*>& SolarSystem::getBodies() {
    return bodies;
}

std::ostream& operator<<(std::ostream& out,const SolarSystem& system) {
    out<<"SOLAR SYSTEM\n";
    for (const auto* bod : system.bodies) {
        out<<bod<<'\n';
    }
    return out;
}