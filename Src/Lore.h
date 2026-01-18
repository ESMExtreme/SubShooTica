#ifndef LORE_H
#define LORE_H

#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <string>

struct LoreEntry {
    int id;
    std::string title;
    std::string content;
};

class Lore {
public:
    Lore();
    ~Lore() = default;
    void handleEvent(const sf::Event& event, int* currentScreen);
    void update(float deltaTime, sf::Vector2u windowSize);
    void draw(sf::RenderWindow& window);

private:
    sf::Font font;
    sf::Font iconFont; // Osobny font dla ikonek (BerlinSans)
    std::optional<sf::Text> titleText;
    std::optional<sf::Text> contentText;

    // System logów
    std::vector<LoreEntry> loreEntries;
    int selectedLoreIndex = 0;
    bool showingLoreList = true; // true = lista, false = wyświetlanie treści

    void loadLoreEntries();
    void updateDisplay();

    // Tło
    sf::Texture backgroundTexture;
    std::optional<sf::Sprite> background;

    // Ikonki (tak jak na mapie i w sklepie)
    sf::Texture loreIconTexture;
    sf::Texture mapIconTexture;
    sf::Texture shopIconTexture;
    std::optional<sf::Sprite> loreIcon;
    std::optional<sf::Sprite> mapIcon;
    std::optional<sf::Sprite> shopIcon;

    // Teksty przy ikonkach
    std::optional<sf::Text> loreText;  // "B"
    std::optional<sf::Text> mapText;   // "N"
    std::optional<sf::Text> shopText;  // "M"
};

#endif // LORE_H

