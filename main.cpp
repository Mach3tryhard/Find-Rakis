#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include "code/Pair.h"
#include "code/Physics.h"
#include "code/Bullet.h"
#include "code/Celestial.h"
#include "code/Generator.h"
#include "code/GUI.h"
#include "code/ParticleSystem.h"
#include "code/Universe.h"
#include "code/Generator.h"

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
    gui.Initialize(window);

    /// CREATE PLAYER
    Physics playerphysics{5};
    SpaceShip player{playerphysics,10,100,100,100};
    player.getShape().setPosition({center.x,center.y});

    /// CREATE UNIVERSE
    Universe universe(25,gen);

    /*sf::Texture texture;
    if (!texture.loadFromFile("textures/Noise1color.png")) {
        return -1;
    }*/
    Generator noise(1000,sf::Color::White);

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
                    view.setSize({1920.f, 1008.f});
                    view.setCenter({static_cast<float>(player.getPhysics().getPosition().x),static_cast<float>(player.getPhysics().getPosition().y)});
                    float windowAspect = static_cast<float>(resize->size.x) / static_cast<float>(resize->size.y);
                    float worldAspect = 1920.f / 1008.f;

                    if (windowAspect > worldAspect) {
                        float width = 1008.f * windowAspect;
                        view.setViewport(sf::FloatRect({(resize->size.x - width)/2.f / resize->size.x, 0.f},{ width / resize->size.x, 1.f}));
                    } else {
                        float height = 1920.f / windowAspect;
                        view.setViewport(sf::FloatRect({0.f, (resize->size.y - height)/2.f / resize->size.y} ,{ 1.f, height / resize->size.y}));
                    }

                    window.setView(view);
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

        universe.Display(player.getPhysics().getPosition(),window,viewRect,noise.getTexture());

        player.getExhaust().update(dt);
        window.draw(player.getExhaust());

        player.UpdateBullets(dt,window,viewRect);
        universe.computeGravity(player);
        /// UPDATE SPACESHIP->CELESTIAL GRAVITY
        gui.DrawText(player);
        /// UPDATE COLLIDERS
        for (auto& system : universe.getSystems()) {
            for (auto& body : system.getBodies()) {
                if (player.getCollider().isCollidingWith(player.getPhysics(),body->getPhysics(),body->getCollider())) {
                    player.getPhysics().setPhysics(player.getCollider().resolveCollision(player.getPhysics(),body->getPhysics(),body->getCollider().getRadius()));
                    player.alignToPlanet(body->getPhysics());
                }
            }
        }
        for (auto& system : universe.getSystems()) {
            for (int i=0;i<system.getBodies().size();i++) {
                system.getBodies()[i]->CheckHit(player.getBullets(),system.getBodies(),i);
            }
        }
        /// DRAW PLAYER, INPUT AND UPDATE PLAYER
        player.InputCheck(dt);
        player.getPhysics().UpdatePhysics(player.getCap(),dt);
        window.draw(player.getShape());
        /// DRAW GUI
        gui.DrawArrowHUD(window, player);
        gui.DrawMiniMap(window,universe,player);
        window.draw(gui.getText());
        window.display();

    }
    return 0;
}
