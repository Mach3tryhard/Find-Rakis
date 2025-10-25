#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>

struct Pair {
    double x,y;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Collider {};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Physics {
private:
    Pair position{};
    Pair velocity{};
    Pair acceleration{};
public:
    /// CONSTRUCTORS
    Physics() {
        position = {0,0};
        velocity = {0,0};
        acceleration = {0,0};
    }
    Physics(const Physics& state) {
        this->position = state.position;
        this->velocity = state.velocity;
        this->acceleration = state.acceleration;
    }
    Physics(Pair position) {
        this->position = position;
        this->velocity = {0,0};
        this->acceleration = {0,0};
    }
    ~Physics() {}
    Physics& operator=(const Physics& state) {
        this->position = state.position;
        this->velocity = state.velocity;
        this->acceleration = state.velocity;
        return *this;
    }
    void UpdatePhysics(float cap) {
        this->velocity.x += this->acceleration.x;
        this->velocity.y += this->acceleration.y;
        float len = sqrtf(velocity.x*velocity.x + velocity.y*velocity.y);
        if (len > cap) {
            this->velocity.x = velocity.x / len * cap;
            this->velocity.y = velocity.y / len * cap;
        }
        this->position.x += this->velocity.x;
        this->position.y += this->velocity.y;
    }
    Pair GetPosition() const {return position;}
    Pair GetVelocity() const {return velocity;}
    Pair GetAcceleration() {return acceleration;}
    void SetAcceleration(Pair acceleration) {this->acceleration = acceleration;}
    void SetPosition(Pair position) {this->position = position;}
    void SetVelocity(Pair velocity) {this->velocity = velocity;}
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
    const float cap=5;
    bool upPressed=false;
public:
    SpaceShip(const Physics& physics, double fuel, double energy, double ore) {
        this->triangle.setOrigin({this->triangle.getRadius(), this->triangle.getRadius()});
        this->physics = physics;
        this->fuel = fuel;
        this->energy = energy;
        this->ore = ore;
    }
    SpaceShip(const Physics& physics,Collider collider, double fuel, double energy, double ore) {
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

    float GetCap() const {
        return cap;
    }
    void InputCheck() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
            triangle.rotate(sf::degrees(-5.f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right)){
            triangle.rotate(sf::degrees(5.f));
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            upPressed = true;
            float angleRad = triangle.getRotation().asRadians();
            angleRad -= 3.14159265f / 2.f;
            Pair direction{std::cos(angleRad), std::sin(angleRad)};
            float thrust = 0.025f;
            direction.x *= thrust;direction.y *= thrust;
            physics.SetAcceleration(direction);
        }else
        if (upPressed) {
            upPressed = false;
            physics.SetAcceleration({0, 0});
        }
    }
    friend std::ostream& operator<<(std::ostream& out,const SpaceShip& ship);
};
std::ostream& operator<<(std::ostream& out,const SpaceShip& ship) {
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
    Bullet(const Physics& physics,Collider collider,int damage) {
        this->collider = collider;
        this->physics = physics;
        this->damage = damage;
    }
    friend std::ostream& operator<<(std::ostream& out,const Bullet& bullet);
};
std::ostream& operator<<(std::ostream& out,const Bullet& bullet) {
    out<<"BULLET\n";
    out<<bullet.physics<<'\n';
    out<<"Stats:\n";
    out<<"Damage:"<<bullet.damage<<'\n';
    return out;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Celestial {
private:
    sf::CircleShape shape;
    Collider collider;
    Physics physics;
    int health,index,color;
    double radius,gravity;
    bool solid;
public:
    Celestial(Physics physics,int index) {
        ///RANDOMLY GENERATED CELESTIAL NOT DONE YET
        this->physics = physics;
        this->health = 10;
        this->index = index;
        this->radius = 30;
        this->gravity = 10;
        this->solid = true;
        shape.setRadius(this->radius);
        shape.setOrigin({static_cast<float>(this->radius), static_cast<float>(this->radius)});
        Pair pos = physics.GetPosition();
        shape.setPosition({static_cast<float>(pos.x), static_cast<float>(pos.y)});
        shape.setFillColor(sf::Color::Blue);
    }
    Celestial(Physics physics,Collider collider,int health,double radius,double gravity,int color,bool solid,int index){
        this->collider = collider;
        this->physics = physics;
        this->health = health;
        this->radius = radius;
        this->gravity = gravity;
        this->color = color;
        this->solid = solid;
        this->index = index;
        shape.setRadius(this->radius);
        shape.setOrigin({static_cast<float>(this->radius), static_cast<float>(this->radius)});
        Pair pos = physics.GetPosition();
        shape.setPosition({static_cast<float>(pos.x), static_cast<float>(pos.y)});
        shape.setFillColor(sf::Color::Blue);
    }
    double& GetRadius() {
        return radius;
    }
    void Display(SpaceShip& player,sf::RenderWindow& window,sf::FloatRect& viewRect) {
        Pair shipPos = player.GetPhysics().GetPosition();
        Pair starPos = physics.GetPosition();
        const sf::Vector2f screenCenter = {static_cast<float>(window.getSize().x/2), static_cast<float>(window.getSize().y/2)};
        float screenX = screenCenter.x + static_cast<float>(starPos.x - shipPos.x);
        float screenY = screenCenter.y + static_cast<float>(starPos.y - shipPos.y);
        if (viewRect.contains({screenX - (float)radius, screenY - (float)radius}) || viewRect.contains({screenX + (float)radius, screenY + (float)radius})) {
            shape.setPosition({screenX, screenY});
            window.draw(shape);
        }
    }
    sf::CircleShape& GetShape() {
        return shape;
    }
    Physics& GetPhysics() {
        return physics;
    }
    friend std::ostream& operator<<(std::ostream& out,Celestial body);
};
std::ostream& operator<<(std::ostream& out,Celestial body){
    out<<"BODY\n";
    out<<body.physics<<'\n';
    out<<"Stats:\n";
    out<<"Index"<<body.index<<'\n'<<"Health:"<<body.health<<'\n'<<"Radius:"<<body.radius<<'\n';
    out<<"Gravity:"<<body.gravity<<'\n';
    out<<"Color:"<<body.color<<'\n'<<"Solid:"<<body.solid<<'\n';
    return out;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SolarSystem {
private:
    Physics physics;
    std::vector<Celestial> bodies;
public:
    SolarSystem(const std::vector<Celestial>& bodies) {
        for (auto& bod : bodies) {
            this->bodies.push_back(bod);
        }
    };
    SolarSystem(const Physics& physics) {
        this->physics = physics;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> d_planets(2, 9);
        int n_planets = d_planets(gen);
        std::uniform_int_distribution<> d_stars(1, 3);
        int n_stars = d_stars(gen);
        std::uniform_int_distribution<> d_asteroid(n_stars+1, n_planets+n_stars);
        //int asteroid_position = d_asteroid(gen);

        /// Generating planets positions
        int r=0;
        for (int i=0; i<=n_stars+n_planets; i++) {
            std::uniform_int_distribution<> distrib_radius(100, 120);
            std::uniform_real_distribution<> distrib(-r, r);
            double posx=distrib(gen);
            double posy= sqrt(r*r-posx*posx);
            if (n_planets%2==0) posy=-posy;
            Pair object_position = {posx,posy};
            Physics newphysics(object_position);
            Celestial newcelestial(newphysics,i);
            bodies.push_back(newcelestial);

            r+=distrib_radius(gen);
        }
    }
    Physics GetPhysics() {
        return physics;
    }
    std::vector<Celestial> GetBodies() {
        return bodies;
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
    const int constaint = 10800;
    const int min_dist = 2000;
    std::vector<SolarSystem> systems;
public:
    Universe(int number) {
        for (int i=1;i<=number;i++) {
            int min = -constaint,max = constaint;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> distrib(min, max);
            Pair pos;
            int gasit=false;
            while (gasit==false) {
                Pair pos = {distrib(gen), distrib(gen)};
                gasit=true;
                for (auto i : systems) {
                    int distance = sqrt(pow(i.GetPhysics().GetPosition().x - pos.x,2)+pow(i.GetPhysics().GetPosition().y - pos.y,2));
                    if ( distance < min_dist)
                        gasit=false;
                }
            }
            Pair system_position = {pos.x,pos.y};
            Physics newphysics(system_position);
            SolarSystem newsystem(newphysics);
            systems.push_back(newsystem);
        }
    }
    Universe(std::vector<SolarSystem> systems) {
        for (auto& system : systems) {
            this->systems.push_back(system);
        }
    };
    std::vector<SolarSystem> GetSystems() {
        return systems;
    }
    ~Universe() {
        systems.clear();
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GUI {
private:
    sf::Font font;
    sf::Text debugText;
public:
    GUI() : debugText(font) {
        this->font = sf::Font("../resources/jetbrains.ttf");
        this->debugText = sf::Text(font);
        this->debugText.setFont(font);
        this->debugText.setCharacterSize(18);
        this->debugText.setFillColor(sf::Color::White);
    }
    sf::Text& GetText() {
        return this->debugText;
    }
    void UpdateGUI(SpaceShip& player) {
        std::string posText ="position: x: " + std::to_string(player.GetPhysics().GetPosition().x)
        +"y: " + std::to_string(player.GetPhysics().GetPosition().y)
        + "\ndirection: "+ std::to_string(player.GetShape().getRotation().asDegrees())
        + "\nvelocity: x " + std::to_string(player.GetPhysics().GetVelocity().x)
        +"y: " + std::to_string(player.GetPhysics().GetVelocity().y)
        + "\nacceleration: x " + std::to_string(player.GetPhysics().GetAcceleration().x)
        +"y: " + std::to_string(player.GetPhysics().GetAcceleration().y);
        debugText.setString(posText);
    }
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode({800, 800}), "My Window", sf::Style::Default);/// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
    window.setFramerateLimit(60);//window.setVerticalSyncEnabled(true);
    sf::View view(sf::FloatRect({0, 0}, {800, 800}));
    window.setView(view);
    sf::Vector2f center = view.getCenter();
    sf::Vector2f size = view.getSize();
    sf::FloatRect viewRect({center.x - size.x/2.f, center.y - size.y/2.f}, {size.x, size.y});
    std::cout << "Fereastra a fost creată\n";

    /// CREATE GUI
    GUI gui{};
    gui.GetText().setPosition({10,10});

    /// CREATE PLAYER
    Physics playerphysics;
    SpaceShip player{playerphysics,100,100,100};
    player.GetShape().setPosition({400.0f,400.0f});

    /// CREATE STAR
    //Physics starphysics;
    //starphysics.SetPosition({50.0f,50.0f});
    //Celestial star(starphysics,0);

    /// CREATE UNIVERSE
    Universe universe(1);

    while(window.isOpen()) {
        bool shouldExit = false;
        while(const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else
                if (event->is<sf::Event::Resized>()) {
                    const auto* resize = event->getIf<sf::Event::Resized>();
                    sf::FloatRect visibleArea({0.f, 0.f}, {static_cast<float>(resize->size.x), static_cast<float>(resize->size.y)});
                    view = sf::View(visibleArea);
                    window.setView(view);
                    viewRect = sf::FloatRect({0, 0}, {static_cast<float>(resize->size.x), static_cast<float>(resize->size.y)});
                    player.GetShape().setPosition({static_cast<float>(resize->size.x) / 2.f, static_cast<float>(resize->size.y) / 2.f});
                    std::cout << "x nou: " << static_cast<float>(resize->size.x) << '\n'
                              << "y nou: " << static_cast<float>(resize->size.y) << '\n';
                }
                else if (event->is<sf::Event::KeyPressed>()) {
                    const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
                    //std::cout << "Received key " << (keyPressed->scancode == sf::Keyboard::Scancode::X ? "X" : "(other)") << "\n";
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
        //using namespace std::chrono_literals;
        //std::this_thread::sleep_for(300ms);
        window.clear();

        /// DRAW CELESTIALS
        //star.Display(player,window,viewRect);
        for (auto i:universe.GetSystems()[0].GetBodies()) {
            i.Display(player,window,viewRect);
        }
        /// DRAW PLAYER, INPUT AND UPDATE PLAYER
        player.InputCheck();
        player.GetPhysics().UpdatePhysics(player.GetCap());
        window.draw(player.GetShape());
        /// DRAW GUI
        gui.UpdateGUI(player);
        window.draw(gui.GetText());

        window.display();
    }
    /// TODO : bug where all solar systems generate at 0,0
    return 0;
}
