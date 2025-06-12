#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <memory>

class TextureManager {
public:
    static TextureManager& getInstance();

    // Loads and stores texture if not already loaded
    sf::Texture* loadTexture(const std::string& name, const std::string& path);

    // Retrieves already loaded texture
    sf::Texture* getTexture(const std::string& name);

private:
    TextureManager() = default;
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
};

#endif