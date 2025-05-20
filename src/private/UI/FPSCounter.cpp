#include "FPSCounter.h"

    void FPSCounter::update(sf::RenderWindow& window) {
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

    void FPSCounter::draw(sf::RenderWindow& window) {
        // Draw in default view so it overlays UI properly
        sf::View originalView = window.getView();

        window.setView(window.getDefaultView());

        window.draw(fpsText);

        window.setView(originalView);

    }

    void FPSCounter::onClick()
    {
    }

    void FPSCounter::setColor(sf::Color col)
    {
        fpsText.setFillColor(col);
    }
