#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <optional>

class GameMap {
public:
    GameMap();
    void draw(sf::RenderWindow& window);
    void CreateTile(const std::string& string, sf::Color color, sf::Vector2f position, int index, int fontSize);
    void handleEvent(const sf::Event& event, int* currentScreen);
    void setSaveSlot(int slot) { currentSaveSlot = slot; }
    int getSaveSlot() const { return currentSaveSlot; }
    int getSelectedLevel() const { return selectedIndex; }
private:
    sf::Texture backgroundTexture;
    std::optional<sf::Sprite> background;
    sf::Font font;
    std::vector<sf::Text> tiles;
    int selectedIndex;
    int currentSaveSlot;
};

#endif // GAMEMAP_H
