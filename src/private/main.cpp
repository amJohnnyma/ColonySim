#include <SFML/Graphics.hpp>
#include <iostream>
#include "structs/worldObjects.h"
#include <SFML/Window/Mouse.hpp>

int main()
{
    unsigned int windowSize = 500;
    int size = 10;
    float cellSize = windowSize/size;        // Pixel size of each grid cell
    sf::RenderWindow window(sf::VideoMode({windowSize, windowSize}), "Grid");
    World world(size,size);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) // Use pollEvent to get events
        {
            if (event.type == sf::Event::Closed) // Check for the closed event
                window.close();
        }
        // Clock to track time between clicks
        sf::Clock clickClock;
        float clickCooldown = 0.3f; // Minimum time between clicks in seconds
        // Check if the left mouse button is pressed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            // Check if enough time has passed since the last click
            if (clickClock.getElapsedTime().asSeconds() <= clickCooldown)
            {
                // Get global mouse position
                sf::Vector2i position = sf::Mouse::getPosition(window);

                // Normalize mouse position to [0.0, 1.0]
                float normX = static_cast<float>(position.x) / windowSize;
                float normY = static_cast<float>(position.y) / windowSize;

                // Scale normalized position to grid coordinates
                int gridX = static_cast<int>(normX * size);
                int gridY = static_cast<int>(normY * size);

                // Clamp to 0–9 range
                gridX = std::min(std::max(gridX, 0), size-1);
                gridY = std::min(std::max(gridY, 0), size-1);

                std::cout << "Mouse: (" << position.x << ", " << position.y << ")\n";
                std::cout << "Normalized: (" << normX << ", " << normY << ")\n";
                std::cout << "Grid coords: (" << gridX << ", " << gridY << ")\n";

                std::unique_ptr<sf::CircleShape> shape = std::make_unique<sf::CircleShape>(cellSize/4);
                shape->setOrigin(cellSize/4,cellSize/4);
                shape->setPosition(
                    gridX * cellSize + cellSize / 2.f,
                    gridY * cellSize + cellSize / 2.f
                );
                shape->setFillColor(sf::Color::Green);
                world.at(gridX, gridY).cs = std::move(shape);

                // Reset the clock to prevent spamming clicks
                clickClock.restart();
            }
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            // Check if enough time has passed since the last click
            if (clickClock.getElapsedTime().asSeconds() <= clickCooldown)
            {
                // Get global mouse position
                sf::Vector2i position = sf::Mouse::getPosition(window);

                // Normalize mouse position to [0.0, 1.0]
                float normX = static_cast<float>(position.x) / windowSize;
                float normY = static_cast<float>(position.y) / windowSize;

                // Scale normalized position to grid coordinates
                int gridX = static_cast<int>(normX * size);
                int gridY = static_cast<int>(normY * size);

                // Clamp to 0–9 range
                gridX = std::min(std::max(gridX, 0), size-1);
                gridY = std::min(std::max(gridY, 0), size-1);

                world.at(gridX, gridY).cs->setFillColor(sf::Color::Red);

                // Reset the clock to prevent spamming clicks
                clickClock.restart();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete))
        {
            // Check if enough time has passed since the last click
            if (clickClock.getElapsedTime().asSeconds() <= clickCooldown)
            {
                // Get global mouse position
                sf::Vector2i position = sf::Mouse::getPosition(window);

                // Normalize mouse position to [0.0, 1.0]
                float normX = static_cast<float>(position.x) / windowSize;
                float normY = static_cast<float>(position.y) / windowSize;

                // Scale normalized position to grid coordinates
                int gridX = static_cast<int>(normX * size);
                int gridY = static_cast<int>(normY * size);

                // Clamp to 0–9 range
                gridX = std::min(std::max(gridX, 0), size-1);
                gridY = std::min(std::max(gridY, 0), size-1);

                if(world.at(gridX, gridY).cs)
                {
                    std::cout << "Resetting shape at (" << gridX << ", " << gridY << ")" << std::endl;
                    world.at(gridX, gridY).cs = std::move(std::make_unique<sf::CircleShape>());
                }

                // Reset the clock to prevent spamming clicks
                clickClock.restart();
            }
        }
        

        window.clear();

        
        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                sf::RectangleShape rs;
                rs.setSize(sf::Vector2f(cellSize, cellSize));
                rs.setPosition(x * cellSize, y * cellSize);
                rs.setOutlineThickness(1.f);
                rs.setOutlineColor(sf::Color::Blue);
                rs.setFillColor(sf::Color::Transparent); // Optional                
        
                window.draw(rs);
                window.draw(*world.at(x,y).cs);
            }
        }
        window.display();
    }
}
