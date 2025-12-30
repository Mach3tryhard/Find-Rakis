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
#include "Exceptions.h"
#include "SpaceShip.h"
#include "Factory.h"

class SolarSystem {
private:
    Physics physics;
    std::vector<Celestial*> bodies;

    sf::SoundBuffer explosionBuffer;
    sf::Sound explosionSound;
public:
    explicit SolarSystem(const Physics& physics,std::mt19937& gen,int number) : explosionSound(explosionBuffer) {

        if (!explosionBuffer.loadFromFile("audio/explosion3.wav")) {
            throw ResourceLoadException("audio/explosion3.wav lipseste");
        }
        explosionSound.setBuffer(explosionBuffer);
        explosionSound.setVolume(20.f);
        explosionSound.setPitch(1.0f);

        this->physics = physics;
        std::uniform_int_distribution<> d_planets(4, 10);
        int n_planets = d_planets(gen);
        int n_stars = 1;

        /// Generating planets positions
        int r = 0;
        for (int i = 0; i < n_stars + n_planets; i++) {
            std::uniform_int_distribution<> distrib_orbit(600, 700);
            std::uniform_real_distribution<> distrib(-r, r);
            std::uniform_real_distribution<> distribBlackHole(0, 100);

            double posx = distrib(gen);
            double spawnHole = distribBlackHole(gen);
            double posy = sqrt(r * r - posx * posx);
            if (n_planets % 2 == 0) posy = -posy;

            Pair object_position = {posx + physics.getPosition().x, posy + physics.getPosition().y};
            Physics newphysics(object_position);

            if (number == 1 && i == 2) {
                Factory::Create(-2, bodies, newphysics, gen, r);
            } else if (spawnHole < 25 && i == 0)
                Factory::Create(-1, bodies, newphysics, gen, r);
            else
                Factory::Create(i, bodies, newphysics, gen, r);

            r += distrib_orbit(gen);
        }
    }

    ~SolarSystem() {
        for (const auto* b : bodies) delete b;
    }
    SolarSystem(const SolarSystem& other) : physics(other.physics), explosionSound(explosionBuffer) {
        if (!explosionBuffer.loadFromFile("audio/explosion3.wav")) {
            throw ResourceLoadException("audio/explosion3.wav lipseste");
        }
        explosionSound.setBuffer(explosionBuffer);
        explosionSound.setVolume(20.f);
        explosionSound.setPitch(1.0f);

        for (const auto *b: other.bodies)
            bodies.push_back(b->clone());
    }

    SolarSystem& operator=(SolarSystem other) {
        swap(*this, other);
        return *this;
    }
    friend void swap(SolarSystem& a, SolarSystem& b) noexcept;
    SolarSystem(SolarSystem&& other) noexcept : physics(other.physics), bodies(other.bodies), explosionSound(explosionBuffer) {
        if (!explosionBuffer.loadFromFile("audio/explosion3.wav")) {
            throw ResourceLoadException("explosion3/button.wav lipseste");
        }
        explosionSound.setBuffer(explosionBuffer);
        explosionSound.setVolume(20.f);
        explosionSound.setPitch(1.0f);

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
    std::vector<Celestial*>& getBodies() { return bodies; }
    void Update(SpaceShip& player,sf::RenderWindow& window,sf::FloatRect& viewRect,sf::Texture &texture,sf::Time dt);
    Physics& getPhysics();
    friend std::ostream& operator<<(std::ostream& out,const SolarSystem& system);
};

#endif //OOP_SOLARSYSTEM_H