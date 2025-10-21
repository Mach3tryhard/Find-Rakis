#include <iostream>
#include <array>
#include <vector>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>

struct Pair {
    double x,y;
};
class Collider {};

class Physics {
private:
    Pair position;
    Pair velocity;
public:
    /// CONSTRUCTORS
    Physics() {}
    Physics(const Physics& state) {
        this->position = state.position;
        this->velocity = state.velocity;
    }
    Physics(Pair position, Pair velocity) {
        this->position = position;
        this->velocity = velocity;
    }
    ~Physics() {}
    /// OPERATORS
    Physics& operator=(const Physics& state) {
        this->position = state.position;
        this->velocity = state.velocity;
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& out,const Physics& state);
    /// FUNCTIONS
    void UpdatePosition() {
        this->position.x += velocity.x;
        this->position.y += velocity.y;

    }
};
std::ostream& operator<<(std::ostream& out, const Physics& state) {
    out<<"Position:X-"<<state.position.x<<" Y-"<<state.position.y<<'\n';
    out<<"Velocity:X-"<<state.velocity.x<<" Y-"<<state.velocity.y<<'\n';
    return out;
}

class Celestial {
private:
    Collider collider;
    Physics physics;
    int health;
    double radius,gravity,gravity_radius;
    int color;
    bool solid;
public:
    Celestial(Physics physics,Collider collider,int health,double radius,double gravity,double gravity_radius,int color,bool solid){
        this->collider = collider;
        this->physics = physics;
        this->health = health;
        this->radius = radius;
        this->gravity = gravity;
        this->gravity_radius = gravity_radius;
        this->color = color;
        this->solid = solid;
    }
    friend std::ostream& operator<<(std::ostream& out,Celestial body);
};
std::ostream& operator<<(std::ostream& out,Celestial body){
    out<<"BODY\n";
    out<<body.physics<<'\n';
    out<<"Stats:\n";
    out<<"Health:"<<body.health<<'\n'<<"Radius:"<<body.radius<<'\n';
    out<<"Gravity:"<<body.gravity<<'\n'<<"Gravity_Radius:"<<body.gravity_radius<<'\n';
    out<<"Color:"<<body.color<<'\n'<<"Solid:"<<body.solid<<'\n';
    return out;
}

class SpaceShip {
private:
    Collider collider;
    Physics physics;
    double direction;
    double fuel,energy,ore;
public:
    SpaceShip(Physics physics,Collider collider,double direction, double fuel, double energy, double ore) {
        this->collider = collider;
        this->physics = physics;
        this->direction = direction;
        this->fuel = fuel;
        this->energy = energy;
        this->ore = ore;
    }
    friend std::ostream& operator<<(std::ostream& out,SpaceShip ship);
};
std::ostream& operator<<(std::ostream& out,SpaceShip ship) {
    out<<"SHIP\n";
    out<<ship.physics<<'\n';
    out<<"Stats:\n";
    out<<"Direction:"<<ship.direction<<'\n';
    out<<"Fuel:"<<ship.fuel<<"Energy:"<<ship.energy<<"ORE:"<<ship.ore<<'\n';
    return out;
}


class Bullet {
private:
    Collider collider;
    Physics physics;
    int damage;
public:
    Bullet(Physics physics,Collider collider,int damage) {
        this->collider = collider;
        this->physics = physics;
        this->damage = damage;
    }
    friend std::ostream& operator<<(std::ostream& out,Bullet bullet);
};
std::ostream& operator<<(std::ostream& out,Bullet bullet) {
    out<<"BULLET\n";
    out<<bullet.physics<<'\n';
    out<<"Stats:\n";
    out<<"Damage:"<<bullet.damage<<'\n';
    return out;
}

class SolarSystem {
private:
    Physics physics;
    std::vector<Celestial> bodies;
public:
    SolarSystem(std::vector<Celestial> bodies) {
        for (auto& bod : bodies) {
            this->bodies.push_back(bod);
        }
    };
    SolarSystem() {
        //TODO: insert random planet generation
    }
    friend std::ostream& operator<<(std::ostream& out,SolarSystem system);
};
std::ostream& operator<<(std::ostream& out,SolarSystem system) {
    out<<"SOLAR SYSTEM\n";
    for (auto& bod : system.bodies) {
        out<<bod<<'\n';
    }
    return out;
}

class Universe {
private:
    std::vector<SolarSystem> systems;
public:
    Universe(std::vector<SolarSystem> systems) {
        for (auto& system : systems) {
            this->systems.push_back(system);
        }
    };
    Universe() {
        //TODO: Random Systems generation
    }
};

int main() {
    ///
    std::cout << "Hello, world!\n";
    std::array<int, 100> v{};
    int nr;
    std::cout << "Introduceți nr: ";
    /////////////////////////////////////////////////////////////////////////
    /// Observație: dacă aveți nevoie să citiți date de intrare de la tastatură,
    /// dați exemple de date de intrare folosind fișierul tastatura.txt
    /// Trebuie să aveți în fișierul tastatura.txt suficiente date de intrare
    /// (în formatul impus de voi) astfel încât execuția programului să se încheie.
    /// De asemenea, trebuie să adăugați în acest fișier date de intrare
    /// pentru cât mai multe ramuri de execuție.
    /// Dorim să facem acest lucru pentru a automatiza testarea codului, fără să
    /// mai pierdem timp de fiecare dată să introducem de la zero aceleași date de intrare.
    ///
    /// Pe GitHub Actions (bife), fișierul tastatura.txt este folosit
    /// pentru a simula date introduse de la tastatură.
    /// Bifele verifică dacă programul are erori de compilare, erori de memorie și memory leaks.
    ///
    /// Dacă nu puneți în tastatura.txt suficiente date de intrare, îmi rezerv dreptul să vă
    /// testez codul cu ce date de intrare am chef și să nu pun notă dacă găsesc vreun bug.
    /// Impun această cerință ca să învățați să faceți un demo și să arătați părțile din
    /// program care merg (și să le evitați pe cele care nu merg).
    ///
    /////////////////////////////////////////////////////////////////////////
    std::cin >> nr;
    /////////////////////////////////////////////////////////////////////////
    for(int i = 0; i < nr; ++i) {
        std::cout << "v[" << i << "] = ";
        std::cin >> v[i];
    }
    std::cout << "\n\n";
    std::cout << "Am citit de la tastatură " << nr << " elemente:\n";
    for(int i = 0; i < nr; ++i) {
        std::cout << "- " << v[i] << "\n";
    }
    ///////////////////////////////////////////////////////////////////////////
    /// Pentru date citite din fișier, NU folosiți tastatura.txt. Creați-vă voi
    /// alt fișier propriu cu ce alt nume doriți.
    /// Exemplu:
    /// std::ifstream fis("date.txt");
    /// for(int i = 0; i < nr2; ++i)
    ///     fis >> v2[i];
    ///
    ///////////////////////////////////////////////////////////////////////////

    sf::RenderWindow window;
    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
    window.create(sf::VideoMode({800, 700}), "My Window", sf::Style::Default);
    ///////////////////////////////////////////////////////////////////////////
    std::cout << "Fereastra a fost creată\n";
    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: mandatory use one of vsync or FPS limit (not both)            ///
    /// This is needed so we do not burn the GPU                            ///
    window.setVerticalSyncEnabled(true);                                    ///
    /// window.setFramerateLimit(60);                                       ///
    ///////////////////////////////////////////////////////////////////////////

    while(window.isOpen()) {
        bool shouldExit = false;

        while(const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                std::cout << "Fereastra a fost închisă\n";
            }
            else if (event->is<sf::Event::Resized>()) {
                std::cout << "New width: " << window.getSize().x << '\n'
                          << "New height: " << window.getSize().y << '\n';
            }
            else if (event->is<sf::Event::KeyPressed>()) {
                const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
                std::cout << "Received key " << (keyPressed->scancode == sf::Keyboard::Scancode::X ? "X" : "(other)") << "\n";
                if(keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    shouldExit = true;
                }
            }
        }
        if(shouldExit) {
            window.close();
            std::cout << "Fereastra a fost închisă (shouldExit == true)\n";
            break;
        }
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(300ms);

        window.clear();
        window.display();
    }

    std::cout << "Programul a terminat execuția\n";
    return 0;
}
