#include "ParticleSystem.h"
#include <cmath>
#include <random>
#include <SFML/Graphics.hpp>

void ParticleSystem::setEmitter(sf::Vector2f position) {
    m_emitter = position;
}
void ParticleSystem::setEmitting(bool flag) {
    m_emitting = flag;
}
void ParticleSystem::setDirection(float degrees) {
    m_baseAngle = degrees;
}
void ParticleSystem::update(sf::Time elapsed)
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

void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = nullptr;
    target.draw(m_vertices, states);
}

void ParticleSystem::resetParticle(std::size_t index)
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