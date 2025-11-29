#include "Menu.h"

Button::Button(float x, float y, const std::string& str, sf::Font& font, Command* cmd)
    : text(font,str,24),idleColor(70, 70, 70), hoverColor(150, 150, 150), command(cmd)  {
    text.setFillColor(sf::Color::White);

    CenterOrigin(text);
    text.setPosition({x, y});

    shape.setSize({300.f, 60.f});
    shape.setFillColor(idleColor);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::White);

    CenterOrigin(shape);
    shape.setPosition({x, y});
}

Button::~Button() {
    delete command;
}

void Button::Update(const sf::Vector2f& mousePos) {
    if (shape.getGlobalBounds().contains(mousePos)) {
        shape.setFillColor(hoverColor);
    } else {
        shape.setFillColor(idleColor);
    }
}

bool Button::IsClicked(const sf::Event& event, const sf::Vector2f& mousePos) {
    if (shape.getGlobalBounds().contains(mousePos)) {
        if (event.is<sf::Event::MouseButtonPressed>()) {
             if (event.getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
                 if (command) command->Execute();
                 return true;
             }
        }
    }
    return false;
}

void Button::Draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

Menu::Menu(float width, float height) : title(font, "FIND RAKIS",60) {
    if (!font.openFromFile("fonts/jetbrains.ttf")) {
        std::cerr << "Font error\n";
    }

    title.setFillColor(sf::Color(254, 159, 35));
    title.setStyle(sf::Text::Bold);

    CenterOrigin(title);
    title.setPosition({width / 2.f, height / 4.f});
}

Menu::~Menu() {
    for (auto btn : buttons) {
        delete btn;
    }
    buttons.clear();
}

void Menu::Update(const sf::Vector2f& mousePos) {
    for (auto btn : buttons) {
        btn->Update(mousePos);
    }
}

void Menu::HandleInput(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    for (auto btn : buttons) {
        if (btn->IsClicked(event, mousePos)) return;
    }
}

void Menu::Draw(sf::RenderWindow& window) {
    window.draw(title);
    for (auto btn : buttons) {
        btn->Draw(window);
    }
}