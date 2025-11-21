#ifndef OOP_UNIVERSE_H
#define OOP_UNIVERSE_H
#include <functional>
#include "SolarSystem.h"
#include "SpaceShip.h"

class Universe {
private:
    const int constraint = 20000;
    const int min_dist = 7000;
    std::vector<SolarSystem> systems;
public:
    explicit Universe(int number,std::mt19937& gen) {
        for (int i=1;i<=number;i++) {
            int min = -constraint,max = constraint;
            std::uniform_real_distribution<> distrib(min, max);
            Pair newpos={0,0};
            bool gasit=false;
            int incercari=0;
            while (gasit==false && incercari++<1000) {
                newpos = {distrib(gen), distrib(gen)};
                gasit=true;
                for (auto &system : systems) {
                    int distance = sqrt(pow(system.getPhysics().getPosition().x - newpos.x,2)+pow(system.getPhysics().getPosition().y - newpos.y,2));
                    if ( distance < min_dist)
                        gasit=false;
                }
            }
            if (gasit==true) {
                Pair system_position = {newpos.x,newpos.y};
                Physics newphysics(system_position);
                systems.push_back(SolarSystem{newphysics, gen});
            }
        }
    }
    explicit Universe(const std::vector<SolarSystem>& systems) {
        for (const auto& system : systems) {
            this->systems.push_back(system);
        }
    };
    ~Universe() = default;
    void Update(SpaceShip& player,sf::RenderWindow& window,sf::FloatRect& viewRect,sf::Texture &texture,sf::Time dt);
    std::vector<SolarSystem>& getSystems();
    friend std::ostream& operator<<(std::ostream& out,const Universe& universe);
};

#endif //OOP_UNIVERSE_H