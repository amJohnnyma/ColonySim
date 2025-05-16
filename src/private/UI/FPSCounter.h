#include <SFML/Graphics.hpp>

class FPSCounter {
private:
    sf::Text fpsText;
    sf::Clock clock;
    int frameCount;
    float elapsedTime;
    int lastFPS;
    const float updateInterval = 1.f; // update every second
public:
    FPSCounter(const sf::Font& font, unsigned int charSize = 24, sf::Color color = sf::Color::Red, sf::Vector2f position = {10.f, 10.f})
        : frameCount(0), elapsedTime(0.f), lastFPS(0)
    {
        fpsText.setFont(font);
        fpsText.setCharacterSize(charSize);
        fpsText.setFillColor(color);
        fpsText.setPosition(position);
    }

    void update() {
        frameCount++;
        elapsedTime += clock.restart().asSeconds();

        if (elapsedTime >= updateInterval) {
            int currentFPS = static_cast<int>(frameCount / elapsedTime);
            if (currentFPS != lastFPS) {
                fpsText.setString("FPS: " + std::to_string(currentFPS));
                lastFPS = currentFPS;
            }
            frameCount = 0;
            elapsedTime = 0.f;
        }
    }

    void draw(sf::RenderWindow& window) {
        // Draw in default view so it overlays UI properly
        sf::View originalView = window.getView();
        window.setView(window.getDefaultView());
        window.draw(fpsText);
        window.setView(originalView);
    }

};
