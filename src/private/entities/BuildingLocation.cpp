#include "BuildingLocation.h"

BuildingLocation::BuildingLocation(int x, int y, std::string name, double maxResource, std::unique_ptr<sf::Sprite> hitbox, Cell *currentCell)
: Location(x,y,name,maxResource,std::move(hitbox), currentCell)
{

}

BuildingLocation::BuildingLocation(int x, int y, std::string name)
{
    this->x = x;
    this->y = y;
    this->name = name;

    auto& manager = TextureManager::getInstance();
    sf::Texture* texture = manager.loadTexture("building", "src/textures/entities/buildings/building.png");
    auto shape = std::make_unique<sf::Sprite>(*texture);
    shape->setColor(sf::Color::Yellow);
    float scaleX = static_cast<float>(conf::cellSize) / texture->getSize().x;
    float scaleY =static_cast<float>(conf::cellSize)/ texture->getSize().y;
    shape->setScale(scaleX, scaleY);

    // shape->setOrigin(
    //     antTexture->getSize().x / 2.f,
    //     antTexture->getSize().y / 2.f
    // );

    // Now position it based on grid
    shape->setPosition(
        (x) * conf::cellSize,
        (y) * conf::cellSize
    );
    this->hitbox = std::move(shape);


}
