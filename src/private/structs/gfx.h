#ifndef GFX_H
#define GFX_H

#include <SFML/Graphics.hpp>


struct window
{
    unsigned int windowWidth, windowHeight;
    int grids;
    sf::RenderWindow* wndw;

    window(int w, int h, int grids) : windowWidth(w), windowHeight(h), grids(grids) 
    {
        wndw = new sf::RenderWindow(sf::VideoMode({windowWidth,windowHeight}), "Window");
    }

    ~window()
    {
        wndw->close();
        delete wndw;
    }

};

#endif GFX_H