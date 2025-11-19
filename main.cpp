#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include "code/Pair.h"
#include "code/Physics.h"
#include "code/Bullet.h"
#include "code/Celestial.h"
#include "code/Exceptions.h"
#include "code/Generator.h"
#include "code/GUI.h"
#include "code/ParticleSystem.h"
#include "code/Universe.h"
#include "code/Generator.h"

int main() {
    try {
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

        /// CREATE TEXTURES
        Generator noise(1000,sf::Color::White,0);

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
                            if (player.getEnergy()>10)
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

            player.SetRefuel(false,{0,0},0);
            /// UNIVERSE STUFF
            universe.Update(player,window,viewRect,noise.getTexture());
            /// PARTICLE STUFF
            player.getExhaust().update(dt);
            window.draw(player.getExhaust());
            /// PLAYER STUFF
            player.UpdateData(dt,player.getPhysics().getPosition());
            player.UpdateBullets(dt,window,viewRect);
            player.InputCheck(dt, window);
            player.HyperLogic(dt,window);
            window.draw(player.getShape());
            /// DRAW GUI
            gui.DrawText(window,player);
            gui.DrawArrowHUD(window, player);
            gui.DrawMiniMap(window,universe,player);
            gui.DrawBars(window,player);

            window.display();
            /// TODO: FIX ACCELERATION DISPLAY NOT WORKING
        }
    }
    catch (const ResourceLoadException& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    catch (const GenerationException& e) {
        std::cerr << e.what() << "\n";
        return 2;
    }
    catch (const PhysicsException& e) {
        std::cerr << e.what() << "\n";
        return 3;
    }
    catch (const std::exception& e) {
        std::cerr << "Nu se stie error: " << e.what() << "\n";
        return -1;
    }
    return 0;
}