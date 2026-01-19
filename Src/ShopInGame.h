#ifndef SHOPINGAME_H
#define SHOPINGAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <optional>
#include <memory>
#include "SaveData.h"

struct ShopItem {
    std::string name;
    std::string description;
    int price;
    int itemIndex; // Indeks w shopItemsPurchased
    std::shared_ptr<sf::Texture> texture;
    std::optional<sf::Sprite> sprite;
    std::optional<sf::Text> nameText;
    std::optional<sf::Text> priceText;
    std::optional<sf::Text> statusText; // "Kupiono" lub pusty
    sf::Vector2f position;
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
    std::optional<sf::Text> scrapText;
    int currentSaveSlot;
    SaveData* saveData;

    // Tło
    sf::Texture backgroundTexture;
    std::optional<sf::Sprite> background;

    // Ikonki (jak na mapie)
    sf::Texture loreIconTexture;
    sf::Texture mapIconTexture;
    sf::Texture shopIconTexture;
    std::optional<sf::Sprite> loreIcon;
    std::optional<sf::Sprite> mapIcon;
    std::optional<sf::Sprite> shopIcon;

    // Teksty przy ikonkach
    std::optional<sf::Text> loreText;  // "B"
    std::optional<sf::Text> mapText;   // "M"
    std::optional<sf::Text> shopText;  // "N"

    // Przedmioty w sklepie
    std::vector<ShopItem> shopItems;
    int selectedItemIndex = 0;

    // Tekst instrukcji
    std::optional<sf::Text> instructionText;

    void initializeShopItems();
    void updateShopItemsDisplay();
    void applyShopBonus(int itemIndex); // Zastosuj efekt przedmiotu
};

#endif // SHOPINGAME_H

