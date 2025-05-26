#include "InputManager.h"

void InputManager::processEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::KeyPressed) {
        if (!keyStates[event.key.code]) {
            keysPressedOnce.insert(event.key.code);
        }
        keyStates[event.key.code] = true;
    } else if (event.type == sf::Event::KeyReleased) {
        keyStates[event.key.code] = false;
        keysPressedOnce.erase(event.key.code);
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        mouseStates[event.mouseButton.button] = true;
        if (event.mouseButton.button == sf::Mouse::Left) {
            mouseStart = sf::Mouse::getPosition(window);
            selecting = true;
        }
    } else if (event.type == sf::Event::MouseButtonReleased) {
        mouseStates[event.mouseButton.button] = false;
        if (event.mouseButton.button == sf::Mouse::Left) {
            mouseEnd = sf::Mouse::getPosition(window);
            selecting = false;
        }
    }
}

void InputManager::update() {
    keysPressedOnce.clear();
}

bool InputManager::isKeyHeld(sf::Keyboard::Key key) const {
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second;
}

bool InputManager::isKeyPressedOnce(sf::Keyboard::Key key) {
    return keysPressedOnce.find(key) != keysPressedOnce.end();
}

bool InputManager::isMouseHeld(sf::Mouse::Button button) const {
    auto it = mouseStates.find(button);
    return it != mouseStates.end() && it->second;
}

std::optional<std::pair<sf::Vector2i, sf::Vector2i>> InputManager::getSelectionBox(const sf::RenderWindow& window) {
    if (!selecting && mouseStart != mouseEnd)
        return std::make_pair(mouseStart, mouseEnd);
    return std::nullopt;
}


