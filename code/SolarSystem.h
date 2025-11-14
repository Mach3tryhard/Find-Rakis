#ifndef OOP_SOLARSYSTEM_H
#define OOP_SOLARSYSTEM_H
#include "Physics.h"
#include "Pair.h"
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Celestial.h"
#include "ParticleSystem.h"
#include "Planet.h"
#include "Star.h"
#include "Asteroid.h"
#include "SpaceShip.h"

class SolarSystem {
private:
    Physics physics;
    std::vector<Celestial*> bodies;
public:
    explicit SolarSystem(const Physics& physics,std::mt19937& gen) {
        this->physics = physics;
        std::uniform_int_distribution<> d_planets(4, 7);
        int n_planets = d_planets(gen);
        int n_stars = 1;

        /// Generating planets positions
        int r=0;
        for (int i=0; i<n_stars+n_planets; i++) {
            std::uniform_int_distribution<> distrib_orbit(400, 640);
            std::uniform_real_distribution<> distrib(-r, r);

            double posx=distrib(gen);
            double posy= sqrt(r*r-posx*posx);
            if (n_planets%2==0) posy=-posy;

            Pair object_position = {posx+physics.getPosition().x,posy+physics.getPosition().y};
            Physics newphysics(object_position);
            if (i==0 ) {
                Star *newstar = new Star(newphysics);
                newstar->initialize(gen);
                bodies.push_back(newstar);
            }
            else if (i==3){
                Asteroid *newasteroid = new Asteroid(newphysics);
                newasteroid->initialize(gen);
                bodies.push_back(newasteroid);
            }
            else {
                Planet *newplanet = new Planet(newphysics);
                newplanet->initialize(gen);
                bodies.push_back(newplanet);
            }

            r+=distrib_orbit(gen);
        }
    }
    ~SolarSystem() {
        for (const auto* b : bodies) delete b;
    }
    SolarSystem(const SolarSystem& other): physics(other.physics) {
        for (const auto* b : other.bodies)
            bodies.push_back(b->clone());
    }
    SolarSystem& operator=(const SolarSystem& other) {
        if (this != &other) {
            // delete existing bodies
            for (const auto* b : bodies) delete b;
            bodies.clear();

            physics = other.physics;
            for (const auto* b : other.bodies)
                bodies.push_back(b->clone());
        }
        return *this;
    }
    SolarSystem(SolarSystem&& other):physics(other.physics),bodies(other.bodies){
        other.bodies.clear();
    }

    SolarSystem& operator=(SolarSystem&& other) noexcept {
        if (this != &other) {
            for (auto* b : bodies) delete b;
            bodies = std::move(other.bodies);
            other.bodies.clear();
            physics = other.physics;
        }
        return *this;
    }
    void Display(Pair player, sf::RenderWindow &window, sf::FloatRect &viewRect, sf::Texture &texture);
    void computeGravity(SpaceShip& player);
    Physics& getPhysics();
    std::vector<Celestial*>& getBodies();
    friend std::ostream& operator<<(std::ostream& out,const SolarSystem& system);
};

#endif //OOP_SOLARSYSTEM_H