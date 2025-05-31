#ifndef TEXT_BOX_SUBMIT_H
#define TEXT_BOX_SUBMIT_H

#include "UIElement.h"
#include "WorldUIElement.h"

class WorldUIElement;


class TextBoxSubmit : public UIElement
{
    private:
        sf::Text text;
        sf::Font font;
        WorldUIElement* textBox;
        WorldUIElement* submitButton;
        bool activelySelected = false;
    public:
        TextBoxSubmit(World* world, int x, int y, int resolution, int radius, int width, int height, std::string function, std::string textArea, std::string purpose);
        ~TextBoxSubmit();
        void draw(sf::RenderWindow& window) override;
        void update(sf::RenderWindow& window) override;
        void onClick() override;
        void setColor(sf::Color col) override;
        void setText(std::string text) override;
        std::string getText() override;
        void setFontSize(int size) override;
        void move(int x, int y) override;

        //move label, textbox, button, individually functions

};

#endif