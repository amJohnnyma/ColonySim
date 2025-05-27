#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include "../game/headers/GlobalVars.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <map>
#include <set>
#include "Controller.h"
#include "PlayerController.h"
#include "SelectionBox.h"

class World;
class InputManager {
private:
    std::map<sf::Keyboard::Key, bool> keyStates;
    std::set<sf::Keyboard::Key> keysPressedOnce;
    std::map<sf::Mouse::Button, bool> mouseStates;

    sf::Vector2i mouseStart;
    sf::Vector2i mouseEnd;
    bool selecting = false;
    Controller* controller;
    SelectionBox* selectionBox;

public:
    void processEvent(const sf::Event& event, sf::RenderWindow& window);
    void update(sf::RenderWindow& window);

    bool isKeyHeld(sf::Keyboard::Key key) const;
    bool isKeyPressedOnce(sf::Keyboard::Key key);
    bool isMouseHeld(sf::Mouse::Button button) const;
    void draw(sf::RenderWindow& window);


    std::optional<std::pair<sf::Vector2i, sf::Vector2i>> getSelectionBox(const sf::RenderWindow& window);

    InputManager(World* world);
    ~InputManager();

};

#endif