#include "TextBoxSubmit.h"

TextBoxSubmit::TextBoxSubmit(World *world, int x, int y, int resolution, int radius, int width, int height, std::string function, std::string textArea, std::string purpose)
: UIElement(world), purpose(purpose)
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
    textBox = new WorldUIElement(world, position.x / conf::cellSize, position.y/conf::cellSize + 1,resolution,radius,width,height, "trackInput","word");
    textBox->setColor(sf::Color(100,100,100,255));
    FunctionArgs sArgs;
    sArgs.element = textBox;   
    textBox->setArgs(sArgs);

    //submission button (optional)
    if(purpose!="null")
    {
        submitButton = new WorldUIElement(world,position.x/conf::cellSize, position.y/conf::cellSize + 2, resolution,radius,width,height, "submitInput", "submit");
        submitButton->setColor(sf::Color(150,150,150,255));
        FunctionArgs sbArgs;
        sbArgs.element = submitButton;   
        submitButton->setArgs(sbArgs);
    }
}

TextBoxSubmit::~TextBoxSubmit()
{
    delete textBox;
    delete submitButton;
}

void TextBoxSubmit::draw(sf::RenderWindow& window) {
    sf::View originalView = window.getView();
    window.setView(window.getDefaultView());
    if(text.getString() != "")
        window.draw(text);
        window.setView(originalView);

    textBox->draw(window);
    submitButton->draw(window);
    window.display();

}
void TextBoxSubmit::update(sf::RenderWindow& window, sf::Event &event) {

    // Check if textbox is selected and event is text input
    if (selected && event.type == sf::Event::TextEntered)
    {
        std::cout << "Selected" << std::endl;
        char entered = static_cast<char>(event.text.unicode);
        
        if (event.text.unicode < 128 && std::isprint(entered)) {
            std::string currentText = textBox->getText();
            currentText += entered;
            textBox->setText(currentText);
            std::cout << entered << std::endl;
        }
        // Optional: handle backspace
        if (event.text.unicode == 8) { // Backspace
            std::string currentText = textBox->getText();
            if (!currentText.empty())
                currentText.pop_back();
            textBox->setText(currentText);
        }
    }
    textBox->update(window,event); 
    submitButton->update(window,event);
}

void TextBoxSubmit::onClick() {
    std::cout << "TB click" << std::endl;
   // submitButton->onClick();
    textBox->onClick();
    selected = true;
    
}
void TextBoxSubmit::setColor(sf::Color col) {
    submitButton->setColor(col);
    textBox->setColor(col);
}
void TextBoxSubmit::setText(std::string text) {
    
}
std::string TextBoxSubmit::getText() {
    return textBox->getText();
}
void TextBoxSubmit::setFontSize(int size) {

}
void TextBoxSubmit::move(int x, int y) {
    text.setPosition(sf::Vector2f(x,y));
    submitButton->move(x,y);
    textBox->move(x,y);
}

std::string TextBoxSubmit::getPurpose()
{
    return purpose;
}
