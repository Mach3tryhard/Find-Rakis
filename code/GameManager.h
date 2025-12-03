#ifndef OOP_GAMEMANAGER_H
#define OOP_GAMEMANAGER_H
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include "Pair.h"
#include "Physics.h"
#include "Bullet.h"
#include "Celestial.h"
#include "Exceptions.h"
#include "Explosion.h"
#include "Generator.h"
#include "GUI.h"
#include "ParticleSystem.h"
#include "Universe.h"
#include "Generator.h"
#include "Starfield.h"
#include "Menu.h"
#include "ShipComputer.h"

class GameManager {
private:
    const float SCR_WIDTH = 1280;
    const float SCR_HEIGHT = 960;

    sf::RenderWindow window;
    sf::View view;
    sf::Vector2f center;
    sf::Vector2f size;
    sf::FloatRect viewRect;
    std::mt19937 gen;

    GUI gui{};

    Physics playerphysics{5};
    SpaceShip player;
    Starfield*starfield = nullptr;
    Universe*universe = nullptr;
    Generator noise;
    ShipComputer computer;

    bool inMenu = true;
    bool isPaused = false;

    Menu mainMenu;
    Menu pauseMenu;
    Menu deathMenu;
    Menu winMenu;

    sf::Clock clock;

    sf::SoundBuffer soundtrackBuffer;
    sf::Sound OSTSound;

    sf::Font gameFont;
    sf::Text scoreText;

    std::vector<std::unique_ptr<Explosion>> explosions;
    bool isplayerexploding =false;
public:
    void resetGame();
    void Run();
    GameManager() : player(playerphysics, 15.f, 100, 100, 100), noise(1000, sf::Color::White, 0), computer(SCR_WIDTH,SCR_HEIGHT),
                    mainMenu(SCR_WIDTH, SCR_HEIGHT, "FIND RAKIS"),
                    pauseMenu(SCR_WIDTH, SCR_HEIGHT, "PAUSED"), deathMenu(SCR_WIDTH, SCR_HEIGHT, "YOU DIED"),
                    winMenu(SCR_WIDTH, SCR_HEIGHT, "YOU WIN"), OSTSound(soundtrackBuffer), scoreText(gameFont) {
        if (!gameFont.openFromFile("fonts/jetbrains.ttf")) {
            std::cerr << "Failed to load font\n";
        }
        scoreText.setFont(gameFont);
        scoreText.setCharacterSize(35);
        scoreText.setFillColor(sf::Color::White);

        if (!soundtrackBuffer.loadFromFile("audio/ambient.mp3")) { std::cerr << "COULD NOT LOAD OST SOUND"; }
        OSTSound.setBuffer(soundtrackBuffer);
        OSTSound.setVolume(20.f);
        OSTSound.setPitch(1.0f);
        OSTSound.setLooping(true);
        OSTSound.play();
        window.create(sf::VideoMode({1280, 960}), "Find Rakis", sf::Style::Titlebar | sf::Style::Close);
        window.setVerticalSyncEnabled(true);
        view = sf::View(sf::FloatRect({0, 0}, {1280, 960}));
        window.setView(view);

        sf::Image icon;
        if (icon.loadFromFile("textures/icon2.png")) {
            window.setIcon({icon.getSize().x, icon.getSize().y}, icon.getPixelsPtr());
        }

        center = view.getCenter();
        size = view.getSize();
        viewRect = sf::FloatRect({center.x - size.x / 2.f, center.y - size.y / 2.f}, {size.x, size.y});
        gen = std::mt19937(std::random_device{}());

        gui.Initialize(window);

        player.getShape().setPosition({center.x, center.y});

        starfield = new Starfield(250, window.getSize());
        universe = new Universe(25, gen);

        mainMenu.AddButton<bool>("PLAY", 450.f, &inMenu, [](bool *val) {
            *val = false;
        });
        mainMenu.AddButton<bool>("REGENERATE", 550.f, &isPaused, [this](bool *val) {
            delete universe;
            delete starfield;

            universe = new Universe(25, gen);
            starfield = new Starfield(250, window.getSize());

            this->resetGame();

            *val = false;
        });
        mainMenu.AddButton<sf::RenderWindow>("EXIT", 650.f, &window, [](sf::RenderWindow *win) {
            win->close();
        });

        pauseMenu.AddButton<bool>("RESUME", 450.f, &isPaused, [](bool *val) {
            *val = false;
        });
        pauseMenu.AddButton<bool>("QUIT", 550.f, &inMenu, [this](bool *val) {
            *val = true;
            isPaused = false;
            this->resetGame();
        });

        deathMenu.AddButton<SpaceShip>("PERSIST", 450.f, &player, [this](SpaceShip *) {
            this->resetGame();
            delete universe;
            delete starfield;

            universe = new Universe(25, gen);
            starfield = new Starfield(250, window.getSize());
        });
        deathMenu.AddButton<bool>("EXIT", 550.f, &inMenu, [this](bool *val) {
            *val = true;
            this->resetGame();
            delete universe;
            delete starfield;

            universe = new Universe(25, gen);
            starfield = new Starfield(250, window.getSize());
        });

        winMenu.AddButton<SpaceShip>("RESTART", 750.f, &player, [this](SpaceShip *) {
            this->resetGame();
            delete universe;
            delete starfield;

            universe = new Universe(25, gen);
            starfield = new Starfield(250, window.getSize());
        });
        winMenu.AddButton<bool>("EXIT", 850.f, &inMenu, [this](bool *val) {
            *val = true;
            this->resetGame();
            delete universe;
            delete starfield;

            universe = new Universe(25, gen);
            starfield = new Starfield(250, window.getSize());
        });
    }

    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;
    ~GameManager() {
        delete universe;
        delete starfield;
    }
};


#endif //OOP_GAMEMANAGER_H