#include "Generator.h"
#include <random>
#include <SFML/Graphics.hpp>

int Generator::generateDitheredPlanet(unsigned int size, sf::Color baseColor) {
    sf::Image img({size, size});
    std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_real_distribution<float> rand01(0.f, 1.f);

    float radius = size / 2.0f;

    for (unsigned y = 0; y < size; ++y)
    {
        for (unsigned x = 0; x < size; ++x)
        {
            float dx = static_cast<float>(x) - radius;
            float dy = static_cast<float>(y) - radius;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist > radius)
            {
                img.setPixel({x, y}, sf::Color(0, 0, 0, 0)); // transparent background
                continue;
            }

            float gradient = 1.0f - (dist / radius);
            gradient = std::pow(gradient, 3.0f); // sharper edges

            float noise = rand01(rng) - 0.5f;
            float mix = std::clamp(gradient + noise * 0.15f, 0.f, 1.f);

            sf::Color darkColor(
                baseColor.r * 0.3f,
                baseColor.g * 0.3f,
                baseColor.b * 0.3f
            );

            sf::Color c(
                static_cast<std::uint8_t>(darkColor.r + (baseColor.r - darkColor.r) * mix),
                static_cast<std::uint8_t>(darkColor.g + (baseColor.g - darkColor.g) * mix),
                static_cast<std::uint8_t>(darkColor.b + (baseColor.b - darkColor.b) * mix),
                255
            );

            img.setPixel({x, y}, c);
        }
    }
    if (!texture.loadFromImage(img)) {
        return -1;
    }
    return 1;
}

int Generator::generatePerlinPlanet(unsigned int size, sf::Color baseColor)
{
    sf::Image img({size, size});
    std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    unsigned seed = rng();

    float radius = size / 2.f;

    for (unsigned y = 0; y < size; ++y)
    {
        for (unsigned x = 0; x < size; ++x)
        {
            float dx = static_cast<float>(x) - radius;
            float dy = static_cast<float>(y) - radius;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist > radius)
            {
                img.setPixel({x, y}, sf::Color(0, 0, 0, 0));
                continue;
            }

            // Radial shading for spherical look
            float gradient = 1.f - (dist / radius);
            gradient = std::pow(gradient, 3.f);

            // --- Adjusted Perlin noise sampling ---
            // Lower frequency = smoother (zoomed out)
            float zoom = 0.015f; // smaller value = more zoomed out
            float n = perlin2D(x * zoom, y * zoom, seed);
            n += 0.5f * perlin2D(x * zoom * 2.f, y * zoom * 2.f, seed + 1);
            n += 0.25f * perlin2D(x * zoom * 4.f, y * zoom * 4.f, seed + 2);
            n /= 1.75f; // normalize

            // combine gradient & noise
            float mix = std::clamp(gradient * 0.7f + n * 0.5f, 0.f, 1.f);

            // color interpolation between darker + base
            sf::Color darkColor(
                static_cast<std::uint8_t>(baseColor.r * 0.25f),
                static_cast<std::uint8_t>(baseColor.g * 0.25f),
                static_cast<std::uint8_t>(baseColor.b * 0.25f)
            );

            sf::Color c(
                static_cast<std::uint8_t>(darkColor.r + (baseColor.r - darkColor.r) * mix),
                static_cast<std::uint8_t>(darkColor.g + (baseColor.g - darkColor.g) * mix),
                static_cast<std::uint8_t>(darkColor.b + (baseColor.b - darkColor.b) * mix),
                255
            );

            img.setPixel({x, y}, c);
        }
    }

    if (!texture.loadFromImage(img))
        return -1;
    return 1;
}

sf::Texture &Generator::getTexture() {
    return texture;
}