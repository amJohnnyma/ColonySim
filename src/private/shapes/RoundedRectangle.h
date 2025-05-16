#ifndef ROUNDED RECTANGLE_H
#define ROUNDED_RECTANGLE_H
#include "Circle.h"
class RoundedRectangle : public Shape // will be a circle with four corners split up
{
    private:
        int width, height;
    public:
        RoundedRectangle(int xr, int yr, double radius, int resolution, int width, int height);

    public:
        void setFillColor(sf::Color col) override;

};

#endif

