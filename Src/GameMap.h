#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <optional>
#include "SaveData.h"

class GameMap {
public:
    GameMap();
    void draw(sf::RenderWindow& window);
    void CreateTile(const std::string& string, sf::Color color, sf::Vector2f position, int index, int fontSize);
    void handleEvent(const sf::Event& event, int* currentScreen);
    void setSaveSlot(int slot);
    int getSaveSlot() const { return currentSaveSlot; }
    int getSelectedLevel() const { return selectedIndex; }
private:
    void loadBackground(int levelIndex);

    sf::Texture currentBackgroundTexture;
    std::optional<sf::Sprite> background;
    sf::Font font;
    std::vector<sf::Text> tiles;
    int selectedIndex;
    int currentSaveSlot;
    SaveData saveData;
};

#endif // GAMEMAP_H
