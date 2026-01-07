#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class SaveScreen {
public:
    SaveScreen();
    ~SaveScreen();
    void draw(sf::RenderWindow& window);
    void CreateTile(const std::string& string, sf::Color color, sf::Vector2f position,int index, int fontSize);
    void handleEvent(const sf::Event& event, int* currentScreen);
    void keyPressHandler(const sf::Event::KeyPressed* key, int* currentScreen);
private:
    sf::Font font;
    std::vector<sf::Text> saveMenu;
    int selectedItemIndex;
    void MoveUp();
    void MoveDown();
};