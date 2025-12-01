#ifndef OOP_FACTORY_H
#define OOP_FACTORY_H
#include "Asteroid.h"
#include "Blackhole.h"
#include "Celestial.h"
#include "Planet.h"
#include "Star.h"
#include "Rakis.h"

class Factory {
public:
    enum class CelestialType {
        Star,
        Planet,
        Asteroid,
        Blackhole,
        Rakis
    };
    static Celestial* CelestialFactory(CelestialType type, const Physics& physics, std::mt19937& gen,float orbitRadius){
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
            case CelestialType::Blackhole:
                body = new Blackhole(physics);
                break;
            case CelestialType::Rakis:
                body = new Rakis(physics);
                break;
        }

        if (body) body->initialize(gen,orbitRadius);
        return body;
    }
    static void Create(int i,std::vector<Celestial*>& bodies,const Physics& newphysics,std::mt19937& gen,float orbitRadius) {
        CelestialType type;
        if (i == -1)
            type = CelestialType::Blackhole;
        else if (i == 0)
            type = CelestialType::Star;
        else if (i == -2)
            type = CelestialType::Rakis;
        else if (i == 3)
            type = CelestialType::Asteroid;
        else
            type = CelestialType::Planet;
        bodies.push_back(CelestialFactory(type, newphysics, gen,orbitRadius));
    }
};

#endif //OOP_FACTORY_H