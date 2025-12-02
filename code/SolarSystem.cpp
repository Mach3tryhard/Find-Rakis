#include "SolarSystem.h"
#include "Physics.h"
#include <cmath>
#include <functional>
#include <SFML/Graphics.hpp>
#include "Celestial.h"
#include "ParticleSystem.h"
#include "SpaceShip.h"

void SolarSystem::Update(SpaceShip& player,sf::RenderWindow& window,sf::FloatRect& viewRect,sf::Texture &texture,sf::Time dt) {
    for (long unsigned int i=0;i<bodies.size();i++) {
        bodies[i]->computeGravity(player);
        bodies[i]->Display(player.getPhysics().getPosition(),window,viewRect,texture);
        bodies[i]->CelestialEffects(window,player.getPhysics().getPosition(),viewRect);
        bodies[i]->CheckHit(player.getBullets(), player);
        if (player.getCollider().isCollidingWith(player.getPhysics(),bodies[i]->getPhysics(),bodies[i]->getCollider())) {
            bodies[i]->isCrashed(player);
            player.alignToPlanet(bodies[i]->getPhysics());
            player.getPhysics().setPhysics(player.getCollider().resolveCollision(player.getPhysics(),bodies[i]->getPhysics(),bodies[i]->getCollider().getRadius()));
        }
        bodies[i]->RefuelCheck(player);
        bodies[i]->getPhysics().OrbitBody(getPhysics().getPosition(),dt);
    }

    for(auto i = 0ull; i < bodies.size(); i++)
        if(bodies[i]->getHealth() <= 0) {
            explosionSound.play();
            delete bodies[i];
            bodies[i] = nullptr;
        }
    std::erase_if(bodies, [](const auto& body) { return body == nullptr; });
}

Physics& SolarSystem::getPhysics() {
    return physics;
}
void swap(SolarSystem& a, SolarSystem& b) noexcept {
    using std::swap;
    swap(a.physics, b.physics);
    swap(a.bodies,  b.bodies);
}
std::ostream& operator<<(std::ostream& out,const SolarSystem& system) {
    out<<"SOLAR SYSTEM\n";
    for (const auto* bod : system.bodies) {
        out<<bod<<'\n';
    }
    return out;
}