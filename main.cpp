#include <iostream>
#include <array>
#include <vector>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include <random>

struct Pair {
    double x,y;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Collider {};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Physics {
private:
    Pair position;
    Pair velocity;
public:
    /// CONSTRUCTORS
    Physics() {
        position = {0,0};
        velocity = {0,0};
    }
    Physics(const Physics& state) {
        this->position = state.position;
        this->velocity = state.velocity;
    }
    Physics(Pair position, Pair velocity) {
        this->position = position;
        this->velocity = velocity;
    }
    ~Physics() {}
    Physics& operator=(const Physics& state) {
        this->position = state.position;
        this->velocity = state.velocity;
        return *this;
    }
    void UpdatePosition() {
        this->position.x += velocity.x;
        this->position.y += velocity.y;
    }
    Pair GetPosition() {
        return position;
    }
    friend std::ostream& operator<<(std::ostream& out,const Physics& state);
};
std::ostream& operator<<(std::ostream& out, const Physics& state) {
    out<<"Position:X-"<<state.position.x<<" Y-"<<state.position.y<<'\n';
    out<<"Velocity:X-"<<state.velocity.x<<" Y-"<<state.velocity.y<<'\n';
    return out;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SpaceShip {
private:
    sf::CircleShape triangle{10.0f,3};
    Collider collider;
    Physics physics;
    double fuel,energy,ore;
public:
    SpaceShip(Physics physics, double fuel, double energy, double ore) {
        this->triangle.setOrigin({this->triangle.getRadius(), this->triangle.getRadius()});
        this->physics = physics;
        this->fuel = fuel;
        this->energy = energy;
        this->ore = ore;
    }
    SpaceShip(Physics physics,Collider collider,double direction, double fuel, double energy, double ore) {
        this->collider = collider;
        this->physics = physics;
        this->fuel = fuel;
        this->energy = energy;
        this->ore = ore;
    }
    sf::CircleShape& GetShape() {
        return triangle;
    }
    Physics& GetPhysics() {
        return physics;
    }
    friend std::ostream& operator<<(std::ostream& out,SpaceShip ship);
};
std::ostream& operator<<(std::ostream& out,SpaceShip ship) {
    out<<"SHIP\n";
    out<<ship.physics<<'\n';
    out<<"Stats:\n";
    out<<"Fuel:"<<ship.fuel<<"Energy:"<<ship.energy<<"ORE:"<<ship.ore<<'\n';
    return out;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Celestial {
private:
    Collider collider;
    Physics physics;
    int health,index,color;
    double radius,gravity,gravity_radius;
    bool solid;
public:
    Celestial(Physics physics,int index) {
        ///RANDOMLY GENERATED CELESTIAL
        this->health = 10;
        this->index = index;
        this->color = 0;
        this->radius = 30;
        this->gravity = 10;
        this->gravity_radius = 50;
        this->solid = true;
    }
    Celestial(Physics physics,Collider collider,int health,double radius,double gravity,double gravity_radius,int color,bool solid,int index){
        this->collider = collider;
        this->physics = physics;
        this->health = health;
        this->radius = radius;
        this->gravity = gravity;
        this->gravity_radius = gravity_radius;
        this->color = color;
        this->solid = solid;
        this->index = index;
    }
    friend std::ostream& operator<<(std::ostream& out,Celestial body);
};
std::ostream& operator<<(std::ostream& out,Celestial body){
    out<<"BODY\n";
    out<<body.physics<<'\n';
    out<<"Stats:\n";
    out<<"Index"<<body.index<<'\n'<<"Health:"<<body.health<<'\n'<<"Radius:"<<body.radius<<'\n';
    out<<"Gravity:"<<body.gravity<<'\n'<<"Gravity_Radius:"<<body.gravity_radius<<'\n';
    out<<"Color:"<<body.color<<'\n'<<"Solid:"<<body.solid<<'\n';
    return out;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
    SolarSystem(Physics physics) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> d_planets(2, 9);
        int n_planets = d_planets(gen);
        std::uniform_int_distribution<> d_stars(1, 3);
        int n_stars = d_stars(gen);
        std::uniform_int_distribution<> d_asteroid(n_stars+1, n_planets+n_stars);
        int asteroid_position = d_asteroid(gen);

        /// Generating planets positions
        int r=0;
        for (int i=0; i<=n_stars; i++) {
            std::uniform_int_distribution<> distrib_radius(100, 120);
            std::uniform_real_distribution<> distrib(-r, r);
            double posx=distrib(gen);
            double posy= sqrt(r*r-posx*posx);
            if (n_planets%2==0) posy=-posy;
            Pair object_position = {posx,posy};
            Pair object_velocity = {0,0};
            Physics newphysics(object_position,object_velocity);
            Celestial newcelestial(newphysics,i);
            bodies.push_back(newcelestial);

            r+=distrib_radius(gen);
        }
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Universe {
private:
    std::vector<SolarSystem> systems;
public:
    Universe(){}
    Universe(std::vector<SolarSystem> systems) {
        for (auto& system : systems) {
            this->systems.push_back(system);
        }
    };
    ~Universe() {
        systems.clear();
    }
    void Create_Universe(const int number) {
        for (int i=1;i<=number;i++) {
            int min = -10800,max = 10800;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> distrib(min, max);
            double posx = distrib(gen);
            double posy = distrib(gen);
            Pair system_position = {posx,posy};
            Pair system_velocity = {0,0};
            Physics newphysics(system_position,system_velocity);
            SolarSystem newsystem(newphysics);
            systems.push_back(newsystem);
        }
    }
    friend std::ostream& operator<<(std::ostream& out,Universe universe);
};
std::ostream& operator<<(std::ostream& out,Universe universe) {
    out<<"THE WHOLE UNIVERSE : \n";
    for (auto& each_system : universe.systems) {
        out<<each_system<<'\n';
    }
    return out;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
    sf::RenderWindow window;
    /// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
    window.create(sf::VideoMode({800, 800}), "My Window", sf::Style::Default);
    std::cout << "Fereastra a fost creată\n";
    window.setFramerateLimit(60);//window.setVerticalSyncEnabled(true);
    sf::View view(sf::FloatRect({0, 0}, {800, 800}));
    window.setView(view);

    sf::Font font("jetbrains.ttf");
    sf::Text debugText(font);
    debugText.setFont(font);
    debugText.setCharacterSize(18);
    debugText.setFillColor(sf::Color::White);
    debugText.setPosition({10, 10});

    Physics physics;
    SpaceShip player{physics,100,100,100};
    player.GetShape().setPosition({400.0f,400.0f});

    while(window.isOpen()) {
        bool shouldExit = false;
        while(const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else
            if (event->is<sf::Event::Resized>()) {
                const auto* resize = event->getIf<sf::Event::Resized>();
                float newWidth = static_cast<float>(resize->size.x);
                float newHeight = static_cast<float>(resize->size.y);
                sf::FloatRect visibleArea({0.f, 0.f}, {newWidth, newHeight});
                view = sf::View(visibleArea);
                window.setView(view);

                player.GetShape().setPosition({newWidth / 2.f, newHeight / 2.f});

                std::cout << "x nou: " << newWidth << '\n'
                          << "y nou: " << newHeight << '\n';
            }
            else if (event->is<sf::Event::KeyPressed>()) {
                const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
                std::cout << "Received key " << (keyPressed->scancode == sf::Keyboard::Scancode::X ? "X" : "(other)") << "\n";
                if(keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    shouldExit = true;
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Up) {
                    std::cout << "Up arrow pressed\n";
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Down) {
                    std::cout << "Down arrow pressed\n";
                }
            }
        }
        if(shouldExit) {
            window.close();
            std::cout << "Fereastra a fost închisă (shouldExit == true)\n";
            break;
        }
        //using namespace std::chrono_literals;
        //std::this_thread::sleep_for(300ms);
        window.clear();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
            player.GetShape().rotate(sf::degrees(-10.f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right)){
            player.GetShape().rotate(sf::degrees(10.f));
        }

        window.draw(player.GetShape());

        std::string posText ="x: " + std::to_string(player.GetPhysics().GetPosition().x) +" y: " + std::to_string(player.GetPhysics().GetPosition().y);
        debugText.setString(posText);
        window.draw(debugText);

        window.display();
    }
    /// TODO : solar systems can generate on eachother
    /// TODO : move spaceship
    /// TODO : show celestial bodies when they are in view
    /// TODO : move things from main into functions
    std::cout << "Programul a terminat execuția\n";
    return 0;
}
