#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <optional>

class Options {
public:
    sf::Vector2u resolution = { 1920, 1080 };
    bool fullscreen = true;
    bool vsync = true;
    int fpsLimit = 60;
    int musicVolume = 50; 
    int sfxVolume = 50;
};

struct BoxConfig {
    sf::Vector2f position;
    sf::Vector2f scale;
};

class OptionsMenu {
public:
    OptionsMenu(sf::Music* music);
    void draw(sf::RenderWindow& window);
    void MoveUp();
    void MoveDown();
    int GetPressedItem() const { return selectedItemIndex; }
    void CreateTile(std::string string, sf::Color color, sf::Vector2f position, int fontSize);
    Options optionsList;
    void keyPressHandler(const sf::Event::KeyPressed* key, int* currentScreen);
    bool shouldApplySettings = false; // Flag dla main.cpp
private:
    sf::Music* musicPtr;
    sf::Font font;
    std::vector<sf::Text> optionsMenu;
    int selectedItemIndex;

    // Tło
    sf::Texture backgroundTexture;
    std::optional<sf::Sprite> backgroundSprite;

    // 7 obrazków opcje_box.png
    sf::Texture boxTexture;
    std::vector<sf::Sprite> boxSprites;
    std::vector<BoxConfig> boxConfigs;
};