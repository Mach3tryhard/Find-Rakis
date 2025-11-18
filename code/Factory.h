#ifndef OOP_FACTORY_H
#define OOP_FACTORY_H
#include "Asteroid.h"
#include "Celestial.h"
#include "Planet.h"
#include "Star.h"

class Factory {
public:
    enum class CelestialType {
        Star,
        Planet,
        Asteroid
    };
    static Celestial* CelestialFactory(CelestialType type, const Physics& physics, std::mt19937& gen){
        Celestial* body = nullptr;

        switch(type) {
            case CelestialType::Star:
                body = new Star(physics);
                break;
            case CelestialType::Planet:
                body = new Planet(physics);
                break;
            case CelestialType::Asteroid:
                body = new Asteroid(physics);
                break;
        }

        if (body) body->initialize(gen);
        return body;
    }
    static void Create(int i,std::vector<Celestial*>& bodies,const Physics& newphysics,std::mt19937& gen) {
        CelestialType type;
        if (i == 0) type = CelestialType::Star;
        else if (i == 3) type = CelestialType::Asteroid;
        else type = CelestialType::Planet;
        bodies.push_back(CelestialFactory(type, newphysics, gen));
    }
};

#endif //OOP_FACTORY_H