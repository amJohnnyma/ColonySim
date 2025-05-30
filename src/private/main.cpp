#include "game/headers/Game.h"
#include "game/headers/GlobalVars.h"
#include <filesystem>

//temp include
#include "utils/headers/TextureManager.h"
int main()
{
   Game game(conf::window_size.x,conf::window_size.y, conf::worldSize.x,conf::worldSize.y);
   game.run();
//    game.fixedrun(); //fixed run to help with memory leaks

    // //test vertices with textures
    // sf::RenderWindow window(sf::VideoMode(800, 600), "Texture Test");

    // // Load texture manually
    // sf::Texture texture;
    // if (!texture.loadFromFile("src/textures/entities/ants/ant.png")) {

    //     std::cerr << "Failed to load texture!" << std::endl;        
    //     return -1;
    // }
    // sf::Vector2u size = texture.getSize();
    // std::cout << "Texture size: " << size.x << "x" << size.y << std::endl;
    // sf::Sprite sprite(texture);
    // float targetSize = 50.0f;
    // float scale = targetSize / size.x;
    // sprite.setScale(scale, scale);
    // // Main loop
    // while (window.isOpen()) {
    //     sf::Event event;
    //     while (window.pollEvent(event)) {
    //         if (event.type == sf::Event::Closed)
    //             window.close();
    //     }

    //     window.clear(sf::Color::Black);
    //     window.draw(sprite); // Pass texture here
    //     window.display();
    // }

    return 0;
}
