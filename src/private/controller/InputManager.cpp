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

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        selectionBox->startDrag(static_cast<sf::Vector2i>(worldPos));
    }

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i start, end;
        if (selectionBox->endDrag(start, end)) {
            // Selection drag is complete — call controller
            controller->selectCells(start, end, {}); // Pass your filter if needed
        }
    }
}

void InputManager::update(sf::RenderWindow& window) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        selectionBox->updateDrag(static_cast<sf::Vector2i>(worldPos));
    }
    keysPressedOnce.clear();
}

void InputManager::draw(sf::RenderWindow& window) {
    selectionBox->draw(window);
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

InputManager::InputManager(World *world)
{
    controller = new PlayerController(world);
    selectionBox = new SelectionBox();
}

InputManager::~InputManager()
{
    delete controller;
    delete selectionBox;
}
