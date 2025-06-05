#include "InputManager.h"

 void InputManager::processEvent(const sf::Event& event, sf::RenderWindow& window)
{
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
    if (event.type == sf::Event::MouseWheelScrolled) {
    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
        // Step 1: Get mouse world position before zoom
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f beforeZoom = window.mapPixelToCoords(pixelPos);

        // Step 2: Get current view
        view = window.getView();

        // Step 3: Apply zoom
        if (event.mouseWheelScroll.delta > 0) {
            view.zoom(1.f / zoomFactor);
            currentZoom /= zoomFactor;
        } else if (event.mouseWheelScroll.delta < 0) {
            view.zoom(zoomFactor);
            currentZoom *= zoomFactor;
        }

        // Step 4: Get mouse world position after zoom
        window.setView(view);
        sf::Vector2f afterZoom = window.mapPixelToCoords(pixelPos);

        // Step 5: Adjust view center to keep mouse anchored
        sf::Vector2f offset = beforeZoom - afterZoom;
        view.move(offset);

        // Final set
        window.setView(view);
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
    selectionBox->draw(window); //The world should draw this to minimizse draw calls
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

InputManager::InputManager(World *world, sf::RenderWindow& window)
{
    controller = new PlayerController(world);
    selectionBox = new SelectionBox();
    view = window.getDefaultView();
}

InputManager::~InputManager()
{
    delete controller;
    delete selectionBox;
}
