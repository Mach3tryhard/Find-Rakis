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
        BulletImpact
    };

    bool update(sf::Time dt);
    void initShipExplosion(sf::Vector2f position, std::mt19937& rng);
    void initCelestialExplosion(sf::Vector2f position, std::mt19937& rng);
    void initImpactExplosion(sf::Vector2f position, std::mt19937& rng);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    Explosion(sf::Vector2f position, Type type) {
        int count = 0;
        if (type == Type::Ship) count = 150;
        else if (type == Type::Celestial) count = 300;
        else count = 20;

        m_vertices.setPrimitiveType(sf::PrimitiveType::Points);
        m_vertices.resize(count);
        m_particles.resize(count);

        std::random_device rd;
        std::mt19937 rng(rd());

        if (type == Type::Ship) {
            initShipExplosion(position, rng);
        } else if (type == Type::Celestial) {
            initCelestialExplosion(position, rng);
        } else {
            initImpactExplosion(position, rng);
        }
    }
};

#endif //OOP_EXPLOSION_H