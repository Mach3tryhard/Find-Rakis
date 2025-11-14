#ifndef OOP_GENERATOR_H
#define OOP_GENERATOR_H

#include <random>
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <cmath>

class Generator {
private:
    sf::Texture texture;
    static float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }
    static float lerp(float a, float b, float t) { return a + t * (b - a); }
    static float hash2D(int x, int y, unsigned seed) {
        unsigned h = x * 374761393u + y * 668265263u + seed * 0x9E3779B9u;
        h = (h ^ (h >> 13u)) * 1274126177u;
        return (h & 0xFFFFFF) / float(0xFFFFFF);
    }

    static float perlin2D(float x, float y, unsigned seed) {
        int xi = int(std::floor(x));
        int yi = int(std::floor(y));
        float xf = x - xi;
        float yf = y - yi;

        float tl = hash2D(xi,     yi,     seed);
        float tr = hash2D(xi + 1, yi,     seed);
        float bl = hash2D(xi,     yi + 1, seed);
        float br = hash2D(xi + 1, yi + 1, seed);

        float u = fade(xf);
        float v = fade(yf);

        float t = lerp(tl, tr, u);
        float b = lerp(bl, br, u);
        return lerp(t, b, v);
    }
public:
    Generator(unsigned int size, sf::Color baseColor,int type) {
        if (type==0)
            generateDitheredPlanet(size, baseColor);
        else
            generatePerlinPlanet(size, baseColor);
    }
    int generateDitheredPlanet(unsigned int size, sf::Color baseColor);
    int generatePerlinPlanet(unsigned int size, sf::Color baseColor);

    sf::Texture &getTexture();
};

#endif //OOP_GENERATOR_H