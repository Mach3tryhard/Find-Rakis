#include "SolarSystem.h"
#include "Physics.h"
#include <cmath>
#include <functional>
#include <SFML/Graphics.hpp>
#include "Celestial.h"
#include "ParticleSystem.h"
#include "SpaceShip.h"

void SolarSystem::Update(SpaceShip& player,sf::RenderWindow& window,sf::FloatRect& viewRect,sf::Texture &texture) {
    for (long unsigned int i=0;i<bodies.size();i++) {
        bodies[i]->Display(player.getPhysics().getPosition(),window,viewRect,texture);
        bodies[i]->computeGravity(player);
        bodies[i]->CelestialEffects(window,player.getPhysics().getPosition(),viewRect);
        bodies[i]->CheckHit(player.getBullets(),bodies,i);
        if (player.getCollider().isCollidingWith(player.getPhysics(),bodies[i]->getPhysics(),bodies[i]->getCollider())) {
            player.getPhysics().setPhysics(player.getCollider().resolveCollision(player.getPhysics(),bodies[i]->getPhysics(),bodies[i]->getCollider().getRadius()));
            player.alignToPlanet(bodies[i]->getPhysics());
        }
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