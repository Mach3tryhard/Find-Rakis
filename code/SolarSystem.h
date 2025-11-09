#ifndef OOP_SOLARSYSTEM_H
#define OOP_SOLARSYSTEM_H
#include "Physics.h"
#include "Pair.h"
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Celestial.h"
#include "ParticleSystem.h"

class SolarSystem {
private:
    Physics physics;
    std::vector<Celestial> bodies;
public:
    explicit SolarSystem(const std::vector<Celestial>& bodies) {
        for (const auto& bod : bodies) {
            this->bodies.push_back(bod);
        }
    };
    explicit SolarSystem(const Physics& physics,std::mt19937& gen) {
        this->physics = physics;
        std::uniform_int_distribution<> d_planets(2, 9);
        int n_planets = d_planets(gen);
        int n_stars = 1;

        /// Generating planets positions
        int r=0;
        for (int i=0; i<n_stars+n_planets; i++) {
            std::uniform_int_distribution<> distrib_orbit(400, 640);
            std::uniform_real_distribution<> distrib(-r, r);
            std::uniform_real_distribution<> distrib_radius(50, 200);

            double posx=distrib(gen);
            double posy= sqrt(r*r-posx*posx);
            if (n_planets%2==0) posy=-posy;

            Pair object_position = {posx+physics.getPosition().x,posy+physics.getPosition().y};
            Physics newphysics(object_position);
            double radius = distrib_radius(gen);
            if (i==0 ) {
                sf::Color color = sf::Color(0xFFB154FF);
                Celestial newcelestial(newphysics,i,color,radius);
                bodies.push_back(newcelestial);
            }
            else {
                sf::Color color = sf::Color(0x8A8A8AFF);
                Celestial newcelestial(newphysics,i,color,radius);
                bodies.push_back(newcelestial);
            }

            r+=distrib_orbit(gen);
        }
    }
    void Display(Pair player,sf::RenderWindow& window,sf::FloatRect& viewRect);
    Physics& getPhysics();
    std::vector<Celestial>& getBodies();
    friend std::ostream& operator<<(std::ostream& out,const SolarSystem& system);
};

#endif //OOP_SOLARSYSTEM_H