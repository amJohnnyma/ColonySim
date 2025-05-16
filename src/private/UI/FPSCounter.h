
#include <SFML/Graphics.hpp>
#include "UIElement.h"

class FPSCounter : public UIElement{
private:
    sf::Text fpsText;
    sf::Clock clock;
    int frameCount;
    float elapsedTime;
    int lastFPS;
    const float updateInterval = 1.f; // update every second
    sf::Font font;

public:
    FPSCounter(unsigned int charSize = 24, sf::Color color = sf::Color::Red, sf::Vector2f position = {10.f, 10.f})
        : frameCount(0), elapsedTime(0.f), lastFPS(0)
    {
        if (!font.loadFromFile("src/fonts/pixel.ttf")) {
            std::cerr << "Could not load font\n";
            return;
        }
        fpsText.setFont(this->font);
        fpsText.setCharacterSize(charSize);
        fpsText.setFillColor(color);
        fpsText.setPosition(position);
    }

    void update(sf::RenderWindow& window) override;

    void draw(sf::RenderWindow& window) override;


    void onClick() override;

};
