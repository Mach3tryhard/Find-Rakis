#include "GameManager.h"
#include "SpaceShip.h"

void GameManager::resetGame(){
    player.getPhysics().setPosition({center.x, center.y});
    player.setLast({center.x, center.y});
    player.getPhysics().setVelocity({0, 0});
    player.getPhysics().setAcceleration({0, 0});

    player.setFuel(100);
    player.setDead(false);
    player.setEnergy(100);
    player.setOre(100);
    player.setTimer(0);
    player.setDistance_travelled(0);
    player.setWon(false);
}

void GameManager::Run() {
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
                ScreenWIN.setFillColor(sf::Color(200, 200, 200, 100));
                window.draw(ScreenWIN);
                player.StopAudio();

                int timeScore = 5000 - (int)player.getTimer();
                if (timeScore < 0) timeScore = 0;
                int oreScore = (int)player.getOre() * 100;
                int fuelScore = (int)player.getFuel() * 50;
                int totalScore = timeScore + oreScore + fuelScore;
                std::string stats = "MISSION ACCOMPLISHED\n\n"
                                    "Time Taken: " + std::to_string((int)player.getTimer()) + "s\n" +
                                    "Distance:   " + std::to_string((int)player.getDistance_travelled()) + " u\n" +
                                    "Ore Bonus:  " + std::to_string(oreScore) + "\n" +
                                    "TOTAL SCORE: " + std::to_string(totalScore);

                scoreText.setString(stats);
                sf::FloatRect bounds = scoreText.getLocalBounds();
                scoreText.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
                scoreText.setPosition({window.getSize().x / 2.f, window.getSize().y * 0.5f});
                window.draw(scoreText);

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
                /// DONT FORGET
                window.draw(player.getShape());

                window.setView(window.getDefaultView());

                sf::RectangleShape dark(sf::Vector2f(window.getSize()));
                dark.setFillColor(sf::Color(0,0,0,150));
                window.draw(dark);

                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                pauseMenu.Update(mousePos);
                pauseMenu.Draw(window);
            }
        }
        window.display();
    }
}