#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <functional>

class Button {
private:
    sf::RectangleShape shape;
        mutable sf::Clock clickClock;  // tracks last click time
    mutable bool wasPressed = false; // to track previous button state
    const sf::Time clickDelay = sf::milliseconds(50);
        std::function<void()> onClick;  // callback for what happens on click

public:
    Button(int x, int y, int width, int height, std::function<void()> clickFunc)
    : onClick(clickFunc) {
        shape.setSize(sf::Vector2f(width, height));
        shape.setPosition(sf::Vector2f(x, y));
    }


    bool isClicked(sf::RenderWindow& window) const {
        bool currentlyPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

        if (currentlyPressed && !wasPressed) {  // mouse just pressed this frame
            sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
            sf::FloatRect buttonBounds = shape.getGlobalBounds();
        if (buttonBounds.contains(static_cast<float>(mousePixelPos.x), static_cast<float>(mousePixelPos.y)))           
            {
                // Check if 50ms have passed since last click
                if (clickClock.getElapsedTime() >= clickDelay) {
                    clickClock.restart();
                    wasPressed = true;
                    if (onClick) {
                        onClick();  // call the backend action
                    }   
                    return true;
                }
            }
        }
        
        if (!currentlyPressed) {
            wasPressed = false;  // reset when mouse released
        }
        return false;
    }
    void setPosition(int x, int y)
    {
        shape.setPosition(x,y);
    }
};


#endif