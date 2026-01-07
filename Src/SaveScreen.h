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
    int getSelectedSaveSlot() const { return lastSelectedSlot; }
private:
    sf::Font font;
    std::vector<sf::Text> saveMenu;
    int selectedItemIndex;
    int lastSelectedSlot; // Przechowuje ostatnio wybrany slot (0, 1, lub 2)
    void MoveUp();
    void MoveDown();
};