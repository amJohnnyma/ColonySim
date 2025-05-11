#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <SFML/Graphics.hpp>

class Shape //abstract
{
    private:
        int xg,yg; //gridspace
        int xr, yr; // window space
        sf::VertexArray va{sf::PrimitiveType::Quads, 4 * 10};

    public:
        Shape();
        virtual std::vector<sf::Vertex> & getVertexArray() = 0;
};


#endif