#ifndef OOP_EXPLOSION_H
#define OOP_EXPLOSION_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <random>
#include <cstdint>

class Explosion : public sf::Drawable, public sf::Transformable {
private:
    struct Particle {
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Time lifetime;
        sf::Time maxLifetime;
        sf::Color color;
    };
    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;
    bool m_finished = false;
public:
    enum class Type {
        Ship,
        Celestial,
        BulletImpact,
        Shockwave,
        Spiral,
        Implosion
    };
    Explosion(sf::Vector2f position, Type type) {
        std::random_device rd;
        std::mt19937 rng(rd());


        int count = 50;
        switch (type) {
            case Type::Ship:        count = 150; break;
            case Type::Celestial:   count = 300; break;
            case Type::BulletImpact:count = 20;  break;
            case Type::Shockwave:   count = 200; break;
            case Type::Spiral:      count = 250; break;
            case Type::Implosion:   count = 150; break;

        }

        m_vertices.setPrimitiveType(sf::PrimitiveType::Points);
        m_vertices.resize(count);
        m_particles.resize(count);

        switch (type) {
            case Type::Ship:
                initShipExplosion(position, rng);
                break;
            case Type::Celestial:
                initCelestialExplosion(position, rng);
                break;
            case Type::BulletImpact:
                initImpactExplosion(position, rng);
                break;
            case Type::Shockwave:
                initShockwaveExplosion(position, rng);
                break;
            case Type::Spiral:
                initSpiralExplosion(position, rng);
                break;
            case Type::Implosion:
                initImplosion(position, rng);
                break;
        }
    }
    bool update(sf::Time dt);
    void initShipExplosion(sf::Vector2f position, std::mt19937& rng);
    void initCelestialExplosion(sf::Vector2f position, std::mt19937& rng);
    void initImpactExplosion(sf::Vector2f position, std::mt19937& rng);
    void initShockwaveExplosion(sf::Vector2f position, std::mt19937& rng);
    void initSpiralExplosion(sf::Vector2f position, std::mt19937& rng);
    void initImplosion(sf::Vector2f position, std::mt19937& rng);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif //OOP_EXPLOSION_H