#ifndef SELECTION_BOX_H
#define SELECTION_BOX_H

#include <SFML/Graphics.hpp>

class SelectionBox {
    bool active = false;
    sf::Vector2i start, end;
    sf::RectangleShape visual;

public:
    void startDrag(sf::Vector2i pos) {
        active = true;
        start = end = pos;
        visual.setFillColor(sf::Color(100, 100, 255, 50)); // translucent blue
        visual.setOutlineColor(sf::Color::Blue);
        visual.setOutlineThickness(1.f);
    }

    void updateDrag(sf::Vector2i pos) {
        if (!active) return;
        end = pos;
        sf::Vector2f size(std::abs(end.x - start.x), std::abs(end.y - start.y));
        sf::Vector2f position(std::min(start.x, end.x), std::min(start.y, end.y));
        visual.setSize(size);
        visual.setPosition(static_cast<sf::Vector2f>(position));
    }

    bool endDrag(sf::Vector2i& outStart, sf::Vector2i& outEnd) {
        if (!active) return false;
        active = false;
        outStart = start;
        outEnd = end;
        return true;
    }

    void draw(sf::RenderWindow& window) {
        if (active)
            window.draw(visual);
    }
};

#endif