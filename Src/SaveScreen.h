#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>

struct SaveBoxConfig {
    sf::Vector2f position;
    sf::Vector2f scale;
};

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

    // Tło
    sf::Texture backgroundTexture;
    std::optional<sf::Sprite> backgroundSprite;

    // 4 boxy zapis_box.png
    sf::Texture boxTexture;
    std::vector<sf::Sprite> boxSprites;
    std::vector<SaveBoxConfig> boxConfigs;
};