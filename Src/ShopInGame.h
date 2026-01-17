#ifndef SHOPINGAME_H
#define SHOPINGAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <optional>
#include "SaveData.h"

struct ShopItem {
    std::string name;
    std::string description;
    int price;
    bool purchased;
};

class ShopInGame {
public:
    ShopInGame();
    ~ShopInGame() = default;
    void handleEvent(const sf::Event& event, int* currentScreen);
    void update(float deltaTime, sf::Vector2u windowSize);
    void draw(sf::RenderWindow& window);
    void setSaveSlot(int slot) { currentSaveSlot = slot; }
    void loadFromSaveData(SaveData* saveDataPtr);

private:
    sf::Font font;
    std::vector<ShopItem> items;
    std::vector<sf::Text> itemTexts;
    std::vector<sf::RectangleShape> itemBoxes;
    std::optional<sf::Text> titleText;
    std::optional<sf::Text> scrapText;
    int selectedIndex;
    int currentSaveSlot;
    SaveData* saveData;

    void createShopItems();
    void updateDisplay();
    void purchaseItem();
};

#endif // SHOPINGAME_H

