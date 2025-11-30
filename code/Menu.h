#ifndef OOP_MENU_H
#define OOP_MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <string>
#include <iostream>

template <typename T>
void CenterOrigin(T& drawable) {
    sf::FloatRect bounds = drawable.getLocalBounds();

    drawable.setOrigin(sf::Vector2f(
        bounds.position.x + bounds.size.x / 2.0f,
        bounds.position.y + bounds.size.y / 2.0f
    ));
}

class Command {
public:
    virtual void Execute() = 0;
    virtual ~Command() = default;
};

template <typename T>
class ActionCommand : public Command {
private:
    T* target;
    std::function<void(T*)> action;
public:
    ActionCommand(T* _target, std::function<void(T*)> _action)
        : target(_target), action(_action) {}

    void Execute() override {
        if (target && action) {
            action(target);
        }
    }
};

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Color idleColor;
    sf::Color hoverColor;
    Command* command;

public:
    Button(float x, float y, const std::string& str, sf::Font& font, Command* cmd);
    ~Button();

    void Update(const sf::Vector2f& mousePos);
    bool IsClicked(const sf::Event& event, const sf::Vector2f& mousePos);
    void Draw(sf::RenderWindow& window);
};

class Menu {
private:
    std::vector<Button*> buttons;
    sf::Font font;
    sf::Text title;

public:
    explicit Menu(float width, float height,const std::string& text);
    ~Menu();

    template <typename T>
    void AddButton(const std::string& str, float y, T* target, std::function<void(T*)> func) {
        Command* cmd = new ActionCommand<T>(target, func);

        buttons.push_back(new Button(title.getPosition().x, y, str, font, cmd));
    }

    void Update(const sf::Vector2f& mousePos);
    void HandleInput(const sf::Event& event, const sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);
};

#endif //OOP_MENU_H