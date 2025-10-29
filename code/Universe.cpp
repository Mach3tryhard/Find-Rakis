#include "Universe.h"

std::vector<SolarSystem>& Universe::getSystems() {
    return systems;
}

std::ostream& operator<<(std::ostream& out,const Universe& universe) {
    out<<"THE WHOLE UNIVERSE : \n";
    for (const auto& each_system : universe.systems) {
        out<<each_system<<'\n';
    }
    return out;
}