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
class ParticleSystem : public sf::Drawable, public sf::Transformable
{
public:
    explicit ParticleSystem(unsigned int count)
        : m_particles(count), m_vertices(sf::PrimitiveType::Points, count)
    {}
    void setEmitter(sf::Vector2f position) {
        m_emitter = position;
    }
    void setEmitting(bool flag) {
        m_emitting = flag;
    }
    void setDirection(float degrees) {
        m_baseAngle = degrees;
    }
    void update(sf::Time elapsed)
    {
        for (std::size_t i = 0; i < m_particles.size(); ++i)
        {
            Particle& p = m_particles[i];
            p.lifetime -= elapsed;

            // Respawn only if particle died AND emitting
            if (p.lifetime <= sf::Time::Zero && m_emitting)
                resetParticle(i);

            // Update position
            p.position += p.velocity * elapsed.asSeconds();
            m_vertices[i].position = p.position;

            // Fade out alpha even if not emitting
            float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
            if (ratio < 0.f) ratio = 0.f;
            m_vertices[i].color.a = static_cast<std::uint8_t>(ratio * 255);
        }
    }

private:
    struct Particle {
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Time lifetime;
    };

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();
        states.texture = nullptr;
        target.draw(m_vertices, states);
    }

    void resetParticle(std::size_t index)
    {
        static std::random_device rd;
        static std::mt19937 rng(rd());

        std::uniform_real_distribution<float> angleDist(m_baseAngle - 0.30f, m_baseAngle + 0.30f);
        float angle = angleDist(rng);

        float speed = std::uniform_real_distribution<float>(50.f, 120.f)(rng);
        m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);

        m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
        m_particles[index].lifetime = sf::milliseconds(std::uniform_int_distribution<int>(100, 600)(rng));
        m_particles[index].position = m_emitter;

        m_vertices[index].position = m_emitter;
        sf::CircleShape shape(50);
        m_vertices[index].color = sf::Color(0x8ab9ffFF);
    }

    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;
    sf::Vector2f m_emitter;
    sf::Time m_lifetime = sf::seconds(0.3f);
    bool m_emitting = false;
    float m_baseAngle = 270.f;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Physics {
private:
    Pair position{};
    Pair velocity{};
    Pair acceleration{};
public:
    /// CONSTRUCTORS
    Physics(): position({0,0}), velocity({0,0}), acceleration({0,0}){}
    Physics(const Physics& state) {
        this->position = state.position;
        this->velocity = state.velocity;
        this->acceleration = state.acceleration;
    }
    explicit Physics(Pair position) {
        this->position = position;
        this->velocity = {0,0};
        this->acceleration = {0,0};
    }
    ~Physics() = default;
    Physics& operator=(const Physics& state) {
        this->position = state.position;
        this->velocity = state.velocity;
        this->acceleration = state.velocity;
        return *this;
    }
    void UpdatePhysics(float cap,sf::Time dt) {
        float delta = dt.asSeconds();
        this->velocity.x += this->acceleration.x * delta;
        this->velocity.y += this->acceleration.y * delta;

        float len = sqrtf(velocity.x*velocity.x + velocity.y*velocity.y);
        if (len > cap) {
            this->velocity.x = velocity.x / len * cap;
            this->velocity.y = velocity.y / len * cap;
        }
        this->position.x += this->velocity.x * delta;
        this->position.y += this->velocity.y * delta;
    }

    Pair getPosition() const {return position;}
    Pair getVelocity() const {return velocity;}
    Pair getAcceleration() {return acceleration;}
    void setAcceleration(Pair acceleration_) {this->acceleration = acceleration_;}
    void setPosition(Pair position_) {this->position = position_;}
    void setVelocity(Pair velocity_) {this->velocity = velocity_;}
    friend std::ostream& operator<<(std::ostream& out,const Physics& state);
};
std::ostream& operator<<(std::ostream& out, const Physics& state) {
    out<<"Position:X-"<<state.position.x<<" Y-"<<state.position.y<<'\n';
    out<<"Velocity:X-"<<state.velocity.x<<" Y-"<<state.velocity.y<<'\n';
    return out;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Bullet {
private:
    sf::CircleShape shape;
    //Collider collider;
    Physics physics;
    const int damage=10;
    const int radius =5;
public:
    static const float speed;
    explicit Bullet(const Physics& physics) {
        shape.setRadius(this->radius);
        shape.setOrigin({static_cast<float>(this->radius), static_cast<float>(this->radius)});
        Pair pos = physics.getPosition();
        shape.setPosition({static_cast<float>(pos.x), static_cast<float>(pos.y)});
        shape.setFillColor(sf::Color::Green);
        this->physics = physics;
    }
    sf::CircleShape& getShape() {return shape;}
    Physics& getPhysics() { return physics; }
    void Display(Pair& position, sf::RenderWindow& window, sf::FloatRect& viewRect) {
        Pair shipPos = position;
        Pair bulletPos = physics.getPosition();
        const sf::Vector2f screenCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);
        float screenX = screenCenter.x + static_cast<float>(bulletPos.x - shipPos.x);
        float screenY = screenCenter.y + static_cast<float>(bulletPos.y - shipPos.y);

        if (viewRect.contains({screenX, screenY})) {
            shape.setPosition({screenX, screenY});
            window.draw(shape);
        }
    }
    friend std::ostream& operator<<(std::ostream& out,const Bullet& bullet);
};
const float Bullet::speed = 300.f;
std::ostream& operator<<(std::ostream& out,const Bullet& bullet) {
    out<<"BULLET\n";
    out<<bullet.physics<<'\n';
    out<<"Stats:\n";
    out<<"Damage:"<<bullet.damage<<'\n';
    return out;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SpaceShip {
private:
    ParticleSystem exhaust{200};
    sf::CircleShape triangle{10.0f,3};
    std::vector<Bullet> bullets;
    Collider collider;
    Physics physics;
    const float thrust = 120.0f;
    double fuel,energy,ore;
    const float cap=300;
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
    sf::CircleShape& getShape() { return triangle; }
    Physics& getPhysics() { return physics; }
    float getCap() const { return cap; }
    ParticleSystem& getExhaust() { return exhaust;}
    std::vector<Bullet>& getBullets() { return bullets; }
    void ShipMove() {
        float angleRad = triangle.getRotation().asRadians();
        angleRad -= 3.14159265f / 2.f;
        Pair direction{std::cos(angleRad) * thrust, std::sin(angleRad) * thrust};
        physics.setAcceleration(direction);
    }
    void ExhaustMove() {
        float angleRad = triangle.getRotation().asRadians();
        float shipRotationDeg = triangle.getRotation().asDegrees();
        sf::Vector2f shipPos(triangle.getPosition().x,triangle.getPosition().y);
        sf::Vector2f offset(-std::cos(angleRad) * 2.f, -std::sin(angleRad) * 2.f);
        float emissionDeg = shipRotationDeg - 90.f + 180.f; // backward from ship nose
        float emissionRad = emissionDeg * 3.14159265f / 180.f;
        exhaust.setEmitter(shipPos + offset);
        exhaust.setDirection(emissionRad);
    }
    void ShootBullet() {
        Pair shipPos = physics.getPosition();
        float angleRad = triangle.getRotation().asRadians();
        angleRad -= 3.14159265f / 2.f;
        Pair bulletVelocity{std::cos(angleRad) * Bullet::speed, std::sin(angleRad) * Bullet::speed};
        Pair bulletPos{shipPos.x, shipPos.y};
        Physics bulletPhysics(bulletPos);
        bulletPhysics.setVelocity(bulletVelocity);

        Bullet newbullet(bulletPhysics);
        bullets.push_back(newbullet);
    }
    void InputCheck(sf::Time dt) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
            triangle.rotate(sf::degrees(-250.f*dt.asSeconds()));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right)){
            triangle.rotate(sf::degrees(250.f*dt.asSeconds()));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            upPressed = true;
            ShipMove();
            ExhaustMove();
        }else
        if (upPressed) {
            upPressed = false;
            physics.setAcceleration({0, 0});
        }
        exhaust.setEmitting(upPressed);
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
class Celestial {
private:
    sf::CircleShape shape;
    Collider collider;
    Physics physics;
    int health,index,color;
    double radius,gravity;
    bool solid;
public:
    Celestial(const Physics& physics,int index,sf::Color color) {
        ///RANDOMLY GENERATED CELESTIAL NOT DONE YET
        this->physics = physics;
        this->health = 10;
        this->index = index;
        this->radius = 150;
        this->gravity = 10;
        this->solid = true;
        shape.setRadius(this->radius);
        shape.setOrigin({static_cast<float>(this->radius), static_cast<float>(this->radius)});
        Pair pos = physics.getPosition();
        shape.setPosition({static_cast<float>(pos.x), static_cast<float>(pos.y)});
        shape.setFillColor(color);
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
        Pair pos = physics.getPosition();
        shape.setPosition({static_cast<float>(pos.x), static_cast<float>(pos.y)});
        shape.setFillColor(sf::Color::Blue);
    }
    double& getRadius() {
        return radius;
    }
    void Display(SpaceShip& player,sf::RenderWindow& window,sf::FloatRect& viewRect) {
        Pair shipPos = player.getPhysics().getPosition();
        Pair starPos = physics.getPosition();
        const sf::Vector2f screenCenter = {static_cast<float>(window.getSize().x/2), static_cast<float>(window.getSize().y/2)};
        float screenX = screenCenter.x + static_cast<float>(starPos.x - shipPos.x);
        float screenY = screenCenter.y + static_cast<float>(starPos.y - shipPos.y);
        if (viewRect.contains({screenX - static_cast<float>(radius), screenY - static_cast<float>(radius)})
            || viewRect.contains({screenX + static_cast<float>(radius), screenY + static_cast<float>(radius)})
            || viewRect.contains({screenX - static_cast<float>(radius), screenY + static_cast<float>(radius)})
            || viewRect.contains({screenX + static_cast<float>(radius), screenY - static_cast<float>(radius)}))
        {
            shape.setPosition({screenX, screenY});
            window.draw(shape);
        }
    }
    sf::CircleShape& getShape() {
        return shape;
    }
    Physics& getPhysics() {
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
    explicit SolarSystem(const std::vector<Celestial>& bodies) {
        for (auto& bod : bodies) {
            this->bodies.push_back(bod);
        }
    };
    explicit SolarSystem(const Physics& physics) {
        this->physics = physics;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> d_planets(2, 9);
        int n_planets = d_planets(gen);
        int n_stars = 1;

        /// Generating planets positions
        int r=0;
        for (int i=0; i<n_stars+n_planets; i++) {
            std::uniform_int_distribution<> distrib_radius(400, 640);
            std::uniform_real_distribution<> distrib(-r, r);

            double posx=distrib(gen);
            double posy= sqrt(r*r-posx*posx);
            if (n_planets%2==0) posy=-posy;

            Pair object_position = {posx+physics.getPosition().x,posy+physics.getPosition().y};
            Physics newphysics(object_position);
            if (i==0 ) {
                sf::Color color = sf::Color(0xFFB154FF);
                Celestial newcelestial(newphysics,i,color);
                bodies.push_back(newcelestial);
            }
            else {
                sf::Color color = sf::Color(0x8A8A8AFF);
                Celestial newcelestial(newphysics,i,color);
                bodies.push_back(newcelestial);
            }

            r+=distrib_radius(gen);
        }
    }
    Physics& getPhysics() {
        return physics;
    }
    std::vector<Celestial> getBodies() {
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
    explicit Universe(int number) {
        for (int i=1;i<=number;i++) {
            int min = -constaint,max = constaint;
            std::random_device rd;
            std::mt19937 gen(rd());
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
    explicit Universe(std::vector<SolarSystem> systems) {
        for (auto& system : systems) {
            this->systems.push_back(system);
        }
    };
    std::vector<SolarSystem>& getSystems() {
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
    sf::Text& getText() {
        return this->debugText;
    }
    void UpdateGUI(SpaceShip& player,SolarSystem solar_system) {
        std::string posText ="position: x: " + std::to_string(player.getPhysics().getPosition().x)
        +"y: " + std::to_string(player.getPhysics().getPosition().y)
        + "\ndirection: "+ std::to_string(player.getShape().getRotation().asDegrees())
        + "\nvelocity: x " + std::to_string(player.getPhysics().getVelocity().x)
        +"y: " + std::to_string(player.getPhysics().getVelocity().y)
        + "\nacceleration: x " + std::to_string(player.getPhysics().getAcceleration().x)
        +"y: " + std::to_string(player.getPhysics().getAcceleration().y)
        +"\nSolar System->\nx: " + std::to_string(solar_system.getPhysics().getPosition().x)
        +"\ny: " + std::to_string(solar_system.getPhysics().getPosition().y);
        debugText.setString(posText);
    }
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode({800, 800}), "My Window", sf::Style::Default);/// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
    window.setFramerateLimit(240);//window.setVerticalSyncEnabled(true);
    sf::View view(sf::FloatRect({0, 0}, {800, 800}));
    window.setView(view);
    sf::Vector2f center = view.getCenter();
    sf::Vector2f size = view.getSize();
    sf::FloatRect viewRect({center.x - size.x/2.f, center.y - size.y/2.f}, {size.x, size.y});
    std::cout << "Fereastra a fost creată\n";

    /// CREATE GUI
    GUI gui{};
    gui.getText().setPosition({10,10});

    /// CREATE PLAYER
    Physics playerphysics;
    SpaceShip player{playerphysics,100,100,100};
    player.getShape().setPosition({400.0f,400.0f});

    /// CREATE UNIVERSE
    Universe universe(1);

    sf::Clock clock;
    while(window.isOpen()) {
        bool shouldExit = false;
        sf::Time dt = clock.restart();
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
                    player.getShape().setPosition({static_cast<float>(resize->size.x) / 2.f, static_cast<float>(resize->size.y) / 2.f});
                    std::cout << "x nou: " << static_cast<float>(resize->size.x) << '\n'
                              << "y nou: " << static_cast<float>(resize->size.y) << '\n';
                }
                else if (event->is<sf::Event::KeyPressed>()) {
                    const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();

                    if(keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                        shouldExit = true;
                    }
                    if(keyPressed->code == sf::Keyboard::Key::C) {
                        player.ShootBullet();
                    }
                }
        }
        if(shouldExit) {
            window.close();
            std::cout << "Fereastra a fost închisă (shouldExit == true)\n";
            break;
        }

        window.clear();
        /// DRAW CELESTIALS
        for (auto& i:universe.getSystems()[0].getBodies()) {
            i.Display(player,window,viewRect);
        }
        /// DRAW PARTICLES
        player.getExhaust().update(dt);
        window.draw(player.getExhaust());
        /// DRAW BULLETS
        for (auto& bullet: player.getBullets()) {
            bullet.getPhysics().UpdatePhysics(Bullet::speed,dt);
            Pair ship_position = player.getPhysics().getPosition();
            bullet.Display(ship_position,window,viewRect);
        }
        /// DRAW PLAYER, INPUT AND UPDATE PLAYER
        player.InputCheck(dt);
        player.getPhysics().UpdatePhysics(player.getCap(),dt);
        window.draw(player.getShape());
        /// DRAW GUI
        gui.UpdateGUI(player,universe.getSystems()[0]);
        window.draw(gui.getText());

        window.display();

    }
    return 0;
}