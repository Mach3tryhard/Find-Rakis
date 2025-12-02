#ifndef OOP_SHIPCOMPUTER_H
#define OOP_SHIPCOMPUTER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <deque>
#include <iostream>

class ShipComputer {
private:
    struct LogEntry {
        std::string message;
        sf::Color color;
        float lifeTime;
        float maxLifeTime;
    };

    std::deque<LogEntry> logs;
    sf::Font font;
    sf::Text textObj;
    unsigned long long maxMessages = 8;
    float messageDuration = 1000.0f;

    sf::RectangleShape SepLine;
public:
    ShipComputer(float width, float height) : textObj(font) {
        if (!font.openFromFile("fonts/jetbrains.ttf")) {
            std::cerr << "Nu exsta le jetbrains font";
        }
        textObj.setFont(font);
        textObj.setCharacterSize(14);

        float boxWidth = 0.f;
        float boxHeight = 220.f;
        float boxX = width - 525.f;
        float boxY = height - 230.f;

        SepLine.setSize({boxWidth, boxHeight});
        SepLine.setPosition({boxX, boxY});
        SepLine.setFillColor(sf::Color(20, 20, 20, 100));
        SepLine.setOutlineColor(sf::Color::White);
        SepLine.setOutlineThickness(1.f);
    }

    void AddLog(const std::string& message, sf::Color color = sf::Color::White) {
        LogEntry entry;
        entry.message = "> " + message;
        entry.color = color;
        entry.lifeTime = messageDuration;
        entry.maxLifeTime = messageDuration;

        logs.push_back(entry);

        if (logs.size() > maxMessages) {
            logs.pop_front();
        }
    }

    void ClearLog() {
        while (logs.size() > 0) {
            logs.pop_front();
        }
    }

    void Update(sf::Time dt) {
        for (auto it = logs.begin(); it != logs.end(); ) {
            it->lifeTime -= dt.asSeconds();
            if (it->lifeTime <= 0) {
                it = logs.erase(it);
            } else {
                ++it;
            }
        }
    }

    void Draw(sf::RenderWindow& window) {
        float x = window.getSize().x - 510.f;
        float y = window.getSize().y - 55.f;
        float lineHeight = 22.f;

        int count = 0;
        for (auto it = logs.rbegin(); it != logs.rend(); ++it) {
            textObj.setString(it->message);

            sf::Color c = it->color;
            float alpha = (it->lifeTime / it->maxLifeTime) * 255;
            if (alpha > 255) alpha = 255;
            c.a = static_cast<std::uint8_t>(alpha);
            textObj.setFillColor(c);

            textObj.setPosition({x, y - (count * lineHeight)});
            window.draw(textObj);
            count++;
        }
        window.draw(SepLine);
    }
};

#endif //OOP_SHIPCOMPUTER_H