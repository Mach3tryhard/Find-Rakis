#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include "code/Pair.h"
#include "code/Physics.h"
#include "code/Bullet.h"
#include "code/Celestial.h"
#include "code/GUI.h"
#include "code/ParticleSystem.h"
#include "code/Universe.h"

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode({1920, 1008}), "Find Rakis", sf::Style::Default);
    window.setVerticalSyncEnabled(true);
    sf::View view(sf::FloatRect({0, 0}, {1920, 1008}));
    window.setView(view);
    sf::Vector2f center = view.getCenter();
    sf::Vector2f size = view.getSize();
    sf::FloatRect viewRect({center.x - size.x/2.f, center.y - size.y/2.f}, {size.x, size.y});
    std::cout << "Fereastra a fost creată\n";

    std::mt19937 gen(std::random_device{}());

    /// CREATE GUI
    GUI gui{};
    gui.getText().setPosition({10,10});
    gui.Initialize(window);

    /// CREATE PLAYER
    Physics playerphysics;
    SpaceShip player{playerphysics,100,100,100};
    player.getShape().setPosition({center.x,center.y});

    /// CREATE UNIVERSE
    Universe universe(25,gen);

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
        for (auto& i: universe.getSystems()) {
            for (auto& j:i.getBodies()) {
                j.Display(player,window,viewRect);
            }
        }
        /// DRAW PARTICLES
        player.getExhaust().update(dt);
        window.draw(player.getExhaust());
        /// DRAW BULLETS
        for (auto& bullet: player.getBullets()) {
            bullet.getPhysics().UpdatePosition(dt);
            Pair ship_position = player.getPhysics().getPosition();
            bullet.Display(ship_position,window,viewRect);
        }
        /// DRAW PLAYER, INPUT AND UPDATE PLAYER
        player.InputCheck(dt);
        player.getPhysics().UpdatePhysics(player.getCap(),dt);
        window.draw(player.getShape());
        /// DRAW GUI
        gui.DrawMiniMap(window,universe,player);
        gui.UpdateGUI(player,universe.getSystems()[1].getBodies()[0]);
        window.draw(gui.getText());
        window.display();

    }
    return 0;
}
