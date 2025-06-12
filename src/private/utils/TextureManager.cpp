#include "TextureManager.h"
#include <iostream>
#include <string>


TextureManager& TextureManager::getInstance() {
    static TextureManager instance;
    return instance;
}

sf::Texture* TextureManager::loadTexture(const std::string& name, const std::string& path) {
    if (textures.find(name) != textures.end())
        return textures[name].get();

    std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(path)) {
        std::cerr << "Failed to load texture from: " << path << std::endl;
        return nullptr;
    }

    sf::Texture* ptr = texture.get();
    textures[name] = std::move(texture);
    return ptr;
}

sf::Texture* TextureManager::getTexture(const std::string& name) {
    if (textures.find(name) != textures.end())
        return textures[name].get();
    
    std::cerr << "Texture not found: " << name << std::endl;
    return nullptr;
}