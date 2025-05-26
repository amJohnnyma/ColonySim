#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include <SFML/Graphics.hpp>
#include <optional>
#include <map>
#include <set>
#include "../../game/headers/GlobalVars.h"

class InputManager {
public:
    void processEvent(const sf::Event& event, sf::RenderWindow& window);
    void update();

    bool isKeyHeld(sf::Keyboard::Key key) const;
    bool isKeyPressedOnce(sf::Keyboard::Key key);
    bool isMouseHeld(sf::Mouse::Button button) const;

    std::optional<std::pair<sf::Vector2i, sf::Vector2i>> getSelectionBox(const sf::RenderWindow& window);

private:
    std::map<sf::Keyboard::Key, bool> keyStates;
    std::set<sf::Keyboard::Key> keysPressedOnce;
    std::map<sf::Mouse::Button, bool> mouseStates;

    sf::Vector2i mouseStart;
    sf::Vector2i mouseEnd;
    bool selecting = false;
};

#endif