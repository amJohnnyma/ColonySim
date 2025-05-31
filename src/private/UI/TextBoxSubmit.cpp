#include "TextBoxSubmit.h"

TextBoxSubmit::TextBoxSubmit(World *world, int x, int y, int resolution, int radius, int width, int height, std::string function, std::string textArea, std::string purpose)
: UIElement(world)
{
    sf::Vector2f position(x*conf::cellSize, y*conf::cellSize);
    //Label
    if(textArea != "")
    {

        if (!font.loadFromFile("src/fonts/pixel.ttf")) {
            std::cerr << "Could not load font\n";
            return;
        }
        text.setFont(this->font);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);
        text.setPosition(position);
        text.setString(textArea);
    }
    //Text enter
    textBox = new WorldUIElement(world, position.x, position.y+height,resolution,radius,width,height, "trackInput","");
    textBox->setColor(sf::Color(10,10,10,100));
    FunctionArgs sArgs;
    sArgs.element = textBox;   
    textBox->setArgs(sArgs);

    //submission button (optional)
    if(purpose!="null")
    {
        submitButton = new WorldUIElement(world, position.x, position.y + (height*2), resolution,radius,width,height, "submitInput", "submit");
        submitButton->setColor(sf::Color(10,10,10,100));
        FunctionArgs sbArgs;
        sbArgs.element = submitButton;   
        submitButton->setArgs(sbArgs);
    }
}

TextBoxSubmit::~TextBoxSubmit()
{
}

void TextBoxSubmit::draw(sf::RenderWindow& window) {
    window.draw(text);
    textBox->draw(window);
    submitButton->draw(window);

}
void TextBoxSubmit::update(sf::RenderWindow& window) {
    textBox->update(window);
    submitButton->update(window);
}
void TextBoxSubmit::onClick() {
    
}
void TextBoxSubmit::setColor(sf::Color col) {

}
void TextBoxSubmit::setText(std::string text) {

}
std::string TextBoxSubmit::getText() {

}
void TextBoxSubmit::setFontSize(int size) {

}
void TextBoxSubmit::move(int x, int y) {

}
