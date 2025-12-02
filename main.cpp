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
#include "code/Starfield.h"
#include "code/Menu.h"

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
        std::cout << "Fereastra creată\n";
        std::mt19937 gen(std::random_device{}());

        /// CREATE GUI
        GUI gui{};
        gui.Initialize(window);

        /// CREATE PLAYER
        Physics playerphysics{5};
        SpaceShip player{playerphysics,10,100,100,100};
        player.getShape().setPosition({center.x,center.y});

        Starfield* starfield = new Starfield(250, window.getSize());
        Universe* universe = new Universe(25, gen);

        /// CREATE TEXTURES
        Generator noise(1000,sf::Color::White,0);

        auto ResetGame = [&](SpaceShip& p) {
            p.getPhysics().setPosition({center.x, center.y});
            p.setLast({center.x, center.y});
            p.getPhysics().setVelocity({0, 0});
            p.getPhysics().setAcceleration({0, 0});

            p.setFuel(100);
            p.setDead(false);
            p.setEnergy(100);
            p.setOre(100);
            p.setTimer(0);
            p.setDistance_travelled(0);
            p.setWon(false);
        };

        // MENUS SETUP
        bool inMenu = true;
        bool isPaused = false;

        Menu mainMenu(window.getSize().x, window.getSize().y,"FIND RAKIS");
        mainMenu.AddButton<bool>("PLAY", 450.f, &inMenu, [](bool* val) {
            *val = false;
        });
        mainMenu.AddButton<bool>("REGENERATE", 550.f, &isPaused, [&](bool* val){
            delete universe;
            delete starfield;

            universe = new Universe(25, gen);
            starfield = new Starfield(250, window.getSize());

            ResetGame(player);

            *val = false;
        });
        mainMenu.AddButton<sf::RenderWindow>("EXIT", 650.f, &window, [](sf::RenderWindow* win) {
            win->close();
        });

        Menu pauseMenu(window.getSize().x, window.getSize().y,"PAUSED");
        pauseMenu.AddButton<bool>("RESUME", 450.f, &isPaused, [](bool* val) {
            *val = false;
        });
        pauseMenu.AddButton<bool>("QUIT", 550.f, &inMenu, [&](bool* val){
            *val = true;
            isPaused = false;
            ResetGame(player);
        });

        Menu deathMenu(window.getSize().x, window.getSize().y,"YOU DIED");
        deathMenu.AddButton<SpaceShip>("PERSIST", 450.f, &player, [&](SpaceShip* p) {
            ResetGame(*p);
            delete universe;
            delete starfield;

            universe = new Universe(25, gen);
            starfield = new Starfield(250, window.getSize());
        });
        deathMenu.AddButton<bool>("EXIT", 550.f, &inMenu, [&](bool* val) {
            *val = true;
            ResetGame(player);
            delete universe;
            delete starfield;

            universe = new Universe(25, gen);
            starfield = new Starfield(250, window.getSize());
        });

        Menu winMenu(window.getSize().x, window.getSize().y,"YOU WIN");
        winMenu.AddButton<SpaceShip>("RESTART", 450.f, &player, [&](SpaceShip* p) {
            ResetGame(*p);
            delete universe;
            delete starfield;

            universe = new Universe(25, gen);
            starfield = new Starfield(250, window.getSize());
        });
        winMenu.AddButton<bool>("EXIT", 550.f, &inMenu, [&](bool* val) {
            *val = true;
            ResetGame(player);
            delete universe;
            delete starfield;

            universe = new Universe(25, gen);
            starfield = new Starfield(250, window.getSize());
        });

        sf::Clock clock;

        while(window.isOpen()) {
            sf::Time dt = clock.restart();

            while(const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                    window.close();
                if (inMenu) {
                    mainMenu.HandleInput(*event, window);
                }
                else if (player.getWon()) {
                    winMenu.HandleInput(*event, window);
                }
                else if (player.getDead()) {
                    deathMenu.HandleInput(*event, window);
                }
                else if (isPaused) {
                    pauseMenu.HandleInput(*event, window);
                    if (event->is<sf::Event::KeyPressed>()) {
                        if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::Escape)
                            isPaused = false;
                    }
                }
                else {
                    if (event->is<sf::Event::Resized>()) {
                        const auto* resize = event->getIf<sf::Event::Resized>();
                        float aspect = 1920.f / 1008.f;
                        float windowAspect = static_cast<float>(resize->size.x) / resize->size.y;

                        float viewWidth, viewHeight;
                        if (windowAspect >= aspect) {
                            viewHeight = 1008.f;
                            viewWidth = 1008.f * windowAspect;
                        } else {
                            viewWidth = 1920.f;
                            viewHeight = 1920.f / windowAspect;
                        }

                        view.setSize({viewWidth, viewHeight});
                        sf::Vector2f newCenter = {static_cast<float>(player.getPhysics().getPosition().x),static_cast<float>(player.getPhysics().getPosition().y)};
                        view.setCenter(newCenter);
                        sf::FloatRect newRect = {{0.f,0.f}, {1.f,1.f}};
                        view.setViewport(newRect);
                        window.setView(view);
                    }

                    else if (event->is<sf::Event::KeyPressed>()) {
                        const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();

                        if(keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                            isPaused = true;
                        }
                        if(keyPressed->code == sf::Keyboard::Key::C) {
                            if (player.getEnergy()>10)
                                player.ShootBullet();
                        }
                    }
                }
            }

            window.clear();
            if (isPaused || player.getDead() || player.getWon()) {
                dt = sf::Time::Zero;
            }
            if (inMenu) {
                window.setView(window.getDefaultView());
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                mainMenu.Update(mousePos);
                mainMenu.Draw(window);
                starfield->Update(dt,window,player);
            }
            else {
                player.SetRefuel(false,{0,0},0,sf::Color::White);
                /// UNIVERSE STUFF
                starfield->Update(dt,window,player);
                universe->Update(player,window,viewRect,noise.getTexture(),dt);
                /// PARTICLE STUFF
                player.getExhaust().update(dt);
                window.draw(player.getExhaust());
                /// DRAW GUI
                gui.DrawText(window,player);
                gui.DrawArrowHUD(window, player);
                gui.DrawMiniMap(window,*universe,player);
                gui.DrawBars(window,player);
                if (!player.getDead() && !isPaused && !player.getWon()) {
                    /// PLAYER STUFF
                    player.UpdateData(dt,player.getPhysics().getPosition());
                    player.UpdateBullets(dt,window,viewRect);
                    player.InputCheck(dt, window);
                    player.HyperLogic(dt,window);
                    window.draw(player.getShape());
                }
                if (player.getWon()) {
                    window.setView(window.getDefaultView());

                    sf::RectangleShape ScreenWIN(sf::Vector2f(window.getSize()));
                    ScreenWIN.setFillColor(sf::Color(200, 200, 200, 150));
                    window.draw(ScreenWIN);
                    player.StopAudio();

                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    winMenu.Update(mousePos);
                    winMenu.Draw(window);
                }
                if (player.getDead()) {
                    window.setView(window.getDefaultView());

                    sf::RectangleShape Screen(sf::Vector2f(window.getSize()));
                    Screen.setFillColor(sf::Color(200, 200, 200, 150));
                    window.draw(Screen);
                    player.StopAudio();

                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    deathMenu.Update(mousePos);
                    deathMenu.Draw(window);
                }
                if (isPaused) {
                    window.setView(window.getDefaultView());

                    sf::RectangleShape dark(sf::Vector2f(window.getSize()));
                    dark.setFillColor(sf::Color(0,0,0,150));
                    window.draw(dark);

                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    pauseMenu.Update(mousePos);
                    pauseMenu.Draw(window);
                    window.draw(player.getShape());
                }
            }

            window.display();
        }
        delete universe;
        delete starfield;
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
