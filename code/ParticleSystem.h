//
// Created by tris on 29.10.2025.
//

#ifndef OOP_PARTICLESYSTEM_H
#define OOP_PARTICLESYSTEM_H
#include <cmath>
#include <random>
#include <SFML/Graphics.hpp>

///PARTICULELE NU SUNT CODUL MEU: https://www.sfml-dev.org/tutorials/3.0/graphics/vertex-array/#example-particle-system
class ParticleSystem : public sf::Drawable, public sf::Transformable
{
public:
    explicit ParticleSystem(unsigned int count)
        : m_particles(count), m_vertices(sf::PrimitiveType::Points, count)
    {}
    void setEmitter(sf::Vector2f position);
    void setEmitting(bool flag);
    void setDirection(float degrees);
    void update(sf::Time elapsed);
private:
    struct Particle {
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Time lifetime;
    };
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void resetParticle(std::size_t index);

    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;
    sf::Vector2f m_emitter;
    sf::Time m_lifetime = sf::seconds(0.3f);
    bool m_emitting = false;
    float m_baseAngle = 270.f;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif //OOP_PARTICLESYSTEM_H