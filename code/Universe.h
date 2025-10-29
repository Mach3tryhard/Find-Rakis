#ifndef OOP_UNIVERSE_H
#define OOP_UNIVERSE_H
#include "SolarSystem.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Universe {
private:
    const int constaint = 10800;
    //const int min_dist = 2000;
    std::vector<SolarSystem> systems;
public:
    explicit Universe(int number) {
        std::random_device rd;
        std::mt19937 gen(rd());
        for (int i=1;i<=number;i++) {
            int min = -constaint,max = constaint;
            std::uniform_real_distribution<> distrib(min, max);
            Pair newpos={0,0};
            /*int gasit=false;
            while (gasit==false) {
                newpos = {distrib(gen), distrib(gen)};
                gasit=true;
                for (auto system : systems) {
                    int distance = sqrt(pow(system.getPhysics().getPosition().x - newpos.x,2)+pow(system.getPhysics().getPosition().y - newpos.y,2));
                    if ( distance < min_dist)
                        gasit=false;
                }
            }*/
            Pair system_position = {newpos.x,newpos.y};
            Physics newphysics(system_position);
            SolarSystem newsystem(newphysics);
            systems.push_back(newsystem);
        }
    }
    explicit Universe(const std::vector<SolarSystem>& systems) {
        for (const auto& system : systems) {
            this->systems.push_back(system);
        }
    };
    ~Universe() {
        systems.clear();
    }
    std::vector<SolarSystem>& getSystems();
    friend std::ostream& operator<<(std::ostream& out,const Universe& universe);
};

#endif //OOP_UNIVERSE_H