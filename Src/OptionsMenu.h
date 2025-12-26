#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include <fstream>
#include <iostream> 
#include <string>
#include <sstream>

class Options {
public:
    sf::Vector2u resolution = { 1920, 1080 };
    bool fullscreen = true;
    bool vsync = true;
    int fpsLimit = 60;
};
class OptionsMenu {
public:
    OptionsMenu();
    void draw(sf::RenderWindow& window);
    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    int GetPressedItem() const { return selectedItemIndex; }
    void CreateTile(std::string string, sf::Color color, sf::Vector2f position, int index, int fontSize);
    Options optionsList;
    void keyPressHandler(const sf::Event::KeyPressed* key, int* currentScreen);
private:
    sf::Font font;
    std::vector<sf::Text> optionsMenu;
    int selectedItemIndex;
};