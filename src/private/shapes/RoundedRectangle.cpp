#include "RoundedRectangle.h"
//#include "RoundedRectangle.h"

RoundedRectangle::RoundedRectangle(int xg, int yg, double radius, int resolution, int width, int height)
: Shape(xg,yg), width(width), height(height)
{
    //for each resolution/4 add a line with width or height
        getVA().setPrimitiveType(sf::Triangles);

    float left = xg * conf::cellSize + conf::cellSize / 2 - width / 2.f;
    float top = yg * conf::cellSize + conf::cellSize / 2 - height / 2.f;
    float right = left + width;
    float bottom = top + height;

    auto addCorner = [&](float cx, float cy, float startAngle, float endAngle) {
        for (int i = 0; i < resolution; ++i) {
            float angle1 = startAngle + (endAngle - startAngle) * (i / (float)resolution);
            float angle2 = startAngle + (endAngle - startAngle) * ((i + 1) / (float)resolution);

            sf::Vector2f center(cx, cy);
            sf::Vector2f p1(cx + radius * std::cos(angle1), cy + radius * std::sin(angle1));
            sf::Vector2f p2(cx + radius * std::cos(angle2), cy + radius * std::sin(angle2));

            getVA().append(sf::Vertex(center));
            getVA().append(sf::Vertex(p1));
            getVA().append(sf::Vertex(p2));
        }
    };

    // Top-left corner (180° to 270°)
    addCorner(left + radius, top + radius, M_PI, 1.5 * M_PI);

    // Top-right corner (270° to 360°)
    addCorner(right - radius, top + radius, 1.5 * M_PI, 2.0 * M_PI);

    // Bottom-right corner (0° to 90°)
    addCorner(right - radius, bottom - radius, 0.0, 0.5 * M_PI);

    // Bottom-left corner (90° to 180°)
    addCorner(left + radius, bottom - radius, 0.5 * M_PI, M_PI);

    // Add rectangles for the sides and center
    // Top rectangle
    getVA().append(sf::Vertex(sf::Vector2f(left + radius, top)));
    getVA().append(sf::Vertex(sf::Vector2f(right - radius, top)));
    getVA().append(sf::Vertex(sf::Vector2f(left + radius, top + radius)));

    getVA().append(sf::Vertex(sf::Vector2f(right - radius, top)));
    getVA().append(sf::Vertex(sf::Vector2f(right - radius, top + radius)));
    getVA().append(sf::Vertex(sf::Vector2f(left + radius, top + radius)));

}