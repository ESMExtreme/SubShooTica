#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class SaveScreen {
public:
    SaveScreen();
    void draw(sf::RenderWindow& window);
    void CreateTile(std::string string, sf::Color color, sf::Vector2f position,int index, int fontSize);
    void keyPressHandler(const sf::Event::KeyPressed* key, int* currentScreen);
private:
    sf::Font font;
    std::vector<sf::Text> saveMenu;
    int selectedItemIndex;
    void MoveUp();
    void MoveDown();
};