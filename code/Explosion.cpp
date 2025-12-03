#include "Explosion.h"

void Explosion::initShipExplosion(sf::Vector2f position, std::mt19937& rng) {
    std::uniform_real_distribution angleDist(0.f, 2.f * 3.14159265f);
    std::uniform_real_distribution speedDist(100.f, 400.f);
    std::uniform_real_distribution lifeDist(0.5f, 1.2f);

    for (std::size_t i = 0; i < m_particles.size(); ++i) {
        float angle = angleDist(rng);
        float speed = speedDist(rng);

        m_particles[i].position = position;
        m_particles[i].velocity = {std::cos(angle) * speed, std::sin(angle) * speed};
        m_particles[i].maxLifetime = sf::seconds(lifeDist(rng));
        m_particles[i].lifetime = m_particles[i].maxLifetime;

        if (i % 3 == 0) m_particles[i].color = sf::Color::White;
        else if (i % 3 == 1) m_particles[i].color = sf::Color(255, 255, 0);
        else m_particles[i].color = sf::Color(255, 100, 0);

        m_vertices[i].position = position;
        m_vertices[i].color = m_particles[i].color;
    }
}

void Explosion::initCelestialExplosion(sf::Vector2f position, std::mt19937& rng) {
    std::uniform_real_distribution<float> angleDist(0.f, 2.f * 3.14159265f);
    std::uniform_real_distribution<float> speedDist(20.f, 150.f);
    std::uniform_real_distribution<float> lifeDist(1.0f, 2.5f);

    for (std::size_t i = 0; i < m_particles.size(); ++i) {
        float angle = angleDist(rng);
        float speed = speedDist(rng);

        m_particles[i].position = position;
        m_particles[i].velocity = {std::cos(angle) * speed, std::sin(angle) * speed};
        m_particles[i].maxLifetime = sf::seconds(lifeDist(rng));
        m_particles[i].lifetime = m_particles[i].maxLifetime;

        if (i % 2 == 0) m_particles[i].color = sf::Color(100, 100, 100);
        else m_particles[i].color = sf::Color(101, 67, 33);

        m_vertices[i].position = position;
        m_vertices[i].color = m_particles[i].color;
    }
}

void Explosion::initImpactExplosion(sf::Vector2f position, std::mt19937& rng) {
    std::uniform_real_distribution<float> angleDist(0.f, 2.f * 3.14159265f);
    std::uniform_real_distribution<float> speedDist(50.f, 100.f);
    std::uniform_real_distribution<float> lifeDist(0.1f, 0.3f);

    for (std::size_t i = 0; i < m_particles.size(); ++i) {
        float angle = angleDist(rng);
        float speed = speedDist(rng);

        m_particles[i].position = position;
        m_particles[i].velocity = {std::cos(angle) * speed, std::sin(angle) * speed};
        m_particles[i].maxLifetime = sf::seconds(lifeDist(rng));
        m_particles[i].lifetime = m_particles[i].maxLifetime;
        m_particles[i].color = sf::Color::Cyan;

        m_vertices[i].position = position;
        m_vertices[i].color = m_particles[i].color;
    }
}

void Explosion::initShockwaveExplosion(sf::Vector2f position, std::mt19937& rng) {
    std::uniform_real_distribution<float> speedDist(150.f, 200.f);
    std::uniform_real_distribution<float> lifeDist(0.5f, 1.0f);

    for (std::size_t i = 0; i < m_particles.size(); ++i) {
        // Calculate angle based on index to ensure perfect distribution circle
        float t = static_cast<float>(i) / static_cast<float>(m_particles.size());
        float angle = t * 2.f * 3.14159265f;

        float speed = speedDist(rng);

        m_particles[i].position = position;

        // Velocity is purely radial
        m_particles[i].velocity = {
            std::cos(angle) * speed,
            std::sin(angle) * speed
        };

        m_particles[i].maxLifetime = sf::seconds(lifeDist(rng));
        m_particles[i].lifetime = m_particles[i].maxLifetime;

        // Shockwave color: Cyan/Blue energy
        m_particles[i].color = sf::Color(100, 255, 255);

        m_vertices[i].position = position;
        m_vertices[i].color = m_particles[i].color;
    }
}

void Explosion::initSpiralExplosion(sf::Vector2f position, std::mt19937& rng) {
    std::uniform_real_distribution speedDist(50.f, 150.f);
    std::uniform_real_distribution lifeDist(1.0f, 2.0f);
    std::uniform_real_distribution armOffset(0.f, 0.5f);

    int arms = 3;

    for (std::size_t i = 0; i < m_particles.size(); ++i) {

        int armIndex = i % arms;
        float armBaseAngle = (static_cast<float>(armIndex) / arms) * 2.f * 3.14159265f;

        float angle = armBaseAngle + armOffset(rng);
        float speed = speedDist(rng);

        m_particles[i].position = position;

        float swirlStrength = 0.5f;
        sf::Vector2f radial = {std::cos(angle), std::sin(angle)};
        sf::Vector2f tangent = {-std::sin(angle), std::cos(angle)};

        m_particles[i].velocity = (radial + (tangent * swirlStrength)) * speed;

        m_particles[i].maxLifetime = sf::seconds(lifeDist(rng));
        m_particles[i].lifetime = m_particles[i].maxLifetime;

        m_particles[i].color = sf::Color(200, 0, 255);

        m_vertices[i].position = position;
        m_vertices[i].color = m_particles[i].color;
    }
}

void Explosion::initImplosion(sf::Vector2f position, std::mt19937& rng) {
    std::uniform_real_distribution angleDist(0.f, 2.f * 3.14159265f);
    std::uniform_real_distribution radiusDist(100.f, 200.f);
    std::uniform_real_distribution speedDist(100.f, 200.f);

    for (std::size_t i = 0; i < m_particles.size(); ++i) {
        float angle = angleDist(rng);
        float startRadius = radiusDist(rng);
        float speed = speedDist(rng);

        sf::Vector2f offset = {
            std::cos(angle) * startRadius,
            std::sin(angle) * startRadius
        };
        m_particles[i].position = position + offset;

        sf::Vector2f direction = -offset;

        float len = std::sqrt(direction.x*direction.x + direction.y*direction.y);
        if (len != 0) direction /= len;

        m_particles[i].velocity = direction * speed;

        float timeToCenter = startRadius / speed;
        m_particles[i].maxLifetime = sf::seconds(timeToCenter);
        m_particles[i].lifetime = m_particles[i].maxLifetime;

        m_particles[i].color = sf::Color(50, 255, 50);

        m_vertices[i].position = m_particles[i].position;
        m_vertices[i].color = m_particles[i].color;
    }
}

bool Explosion::update(sf::Time dt) {
    if (m_finished) return false;

    int aliveParticles = 0;

    for (std::size_t i = 0; i < m_particles.size(); ++i) {
        if (m_particles[i].lifetime <= sf::Time::Zero) {
            m_vertices[i].color = sf::Color::Transparent;
            continue;
        }

        aliveParticles++;

        m_particles[i].position += m_particles[i].velocity * dt.asSeconds();
        m_vertices[i].position = m_particles[i].position;

        m_particles[i].lifetime -= dt;

        float ratio = m_particles[i].lifetime.asSeconds() / m_particles[i].maxLifetime.asSeconds();
        if (ratio < 0.f) ratio = 0.f;
        
        sf::Color c = m_particles[i].color;
        c.a = static_cast<std::uint8_t>(ratio * 255);
        m_vertices[i].color = c;
    }

    if (aliveParticles == 0) {
        m_finished = true;
        return false;
    }

    return true;
}

void Explosion::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = nullptr;
    target.draw(m_vertices, states);
}