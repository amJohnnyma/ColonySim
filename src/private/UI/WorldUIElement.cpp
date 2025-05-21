#include "WorldUIElement.h"

WorldUIElement::WorldUIElement(World* world, int x, int y, int resolution, int radius, int width, int height, std::string function, std::string textArea)
 : UIElement(world), shape(nullptr), button(nullptr), updateFunc(nullptr)
{

   // std::cout << "rect" << std::endl;
    shape = new RoundedRectangle(x,y,radius,resolution,width, height);
    shape->setFillColor(sf::Color::White);
    this->function = function;
    this->args = FunctionArgs{};

    if (function != "null")
    {

        auto &map = getFunctionMap();
        auto it = map.find(function);
        if (it != map.end())
        {
            auto func = it->second;
            if (function.find("update") == std::string::npos)
            {
                button = new Button(
                    x * conf::cellSize,
                    y * conf::cellSize,
                    width,
                    height,
                    [this, func, world]()
                    {
                        func(world, this->args);
                    });
            }
            else
            {
                this->updateFunc = func;
            }
        }
    }
    if(textArea != "")
    {

        if (!font.loadFromFile("src/fonts/pixel.ttf")) {
            std::cerr << "Could not load font\n";
            return;
        }
        text.setFont(this->font);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);
        text.setPosition(sf::Vector2f{x * conf::cellSize, y*conf::cellSize});
        text.setString(textArea);
    }

}

WorldUIElement::~WorldUIElement()
{
    updateFunc = nullptr;
    args.element = std::nullopt; // reset optional safely
    if (shape) {
        delete shape;
        shape = nullptr;
    }
    if (button) {
        delete button;
        button = nullptr;
    }

}

void WorldUIElement::draw(sf::RenderWindow& window)
{
    // Draw in default view so it overlays UI properly
    sf::View originalView = window.getView();
    window.setView(window.getDefaultView());
    shape->draw(window);
    if(text.getString() != "")
    window.draw(text);
    window.setView(originalView);
}



void WorldUIElement::update(sf::RenderWindow& window)
{
    if(button)
    {
        if(button->isClicked(window))
        {
            onClick();
        }
    }
    if(updateFunc && this->args.element)
    {
     //   std::cout << "updatefunc" << std::endl;
        updateFunc(this->world, this->args);
    }
}

void WorldUIElement::onClick()
{
   // std::cout << "Rect clicked" << std::endl;
}

const std::unordered_map<std::string, std::function<void(World*, const FunctionArgs&)>>& WorldUIElement::getFunctionMap() {
   static const std::unordered_map<std::string, std::function<void(World*, const FunctionArgs&)>> functionMap = {
        {"testClick", [](World* w, const FunctionArgs&) {
            w->testClick();
        }},
        {"spawnEntity", [](World* w, const FunctionArgs& args) {
            if (args.name && args.count) {
                w->spawn(args.name.value(), args.count.value());
            }
        }},
        {"toggleSimState", [](World* w, const FunctionArgs& args){
            w->toggleSimState();
          //  std::cout << "Calling setColor on UIElement at " << *args.element << std::endl;
            bool running = w->isRunning();            
            if (args.element.has_value() && args.element.value() != nullptr) {
                UIElement* elem = args.element.value();
             //   std::cout << "Lambda called with element pointer: " << elem << std::endl;
                if (elem) {
                    std::string text = elem->getText();
                    elem->setColor(running ? sf::Color::Green : sf::Color::Red);
                    elem->setText(text == "Start" ? "Pause" : "Start");
                } else {
                    std::cout << "element pointer is null\n";
                }
            } else {
                std::cout << "args.element not set\n";
            }
        }},
        {"updateWorldStats", [](World* w, const FunctionArgs& args){
          //  std::cout << "Getting tracked vars.." << std::endl;
            TrackedVariables tv = *w->getWorldStats();
          //  std::cout << "Checking element validitry" <<std::endl;
            if (args.element.has_value() && args.element.value() != nullptr) {
                UIElement* elem = args.element.value();
            //    std::cout << "Making magic" << std::endl;
                if (elem) {
              //      std::cout << "Magic trying" << std::endl;
                    elem->setText("Stats:\nBase: " + std::to_string(tv.getBaseFood()));
              //      std::cout << "Made magic" << std::endl;
                } else {
                    std::cout << "No magic today" << std::endl;
                  elem->setText("Stats:\nBase: 0");
                }
            } else {
                std::cout << "args.element not set\n";
            }
        }},
    };
    return functionMap;
}

void WorldUIElement::setColor(sf::Color col)
{
    shape->setFillColor(col);
}

void WorldUIElement::setText(std::string text)
{
    this->text.setString(text);
}

std::string WorldUIElement::getText()
{
    return this->text.getString();
}
