 #ifndef LEVEL2_H
 #define LEVEL2_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <optional>
#include "SaveData.h"
#include "EnemyConfig.h"

struct Background2 {
    sf::Texture texture;
    sf::Sprite sprite;
    float offsetY = 0.f;
    float scrollSpeed = 150.f;
    int difficultyLevel = 2;

    Background2() : sprite(texture) {}
    float getHeight() const { return texture.getSize().y; }
};

struct Bullet2 {
    sf::CircleShape shape;
    sf::Vector2f velocity;
};

struct Enemy2 {
    sf::RectangleShape shape;
    std::optional<sf::Sprite> sprite;
    sf::Vector2f velocity;
    int hp = 1; // HP przeciwnika
    bool useTexture = false; // true = sprite, false = shape
};

class Level2 {
public:
    Level2();
    ~Level2() = default;
    void handleEvent(const sf::Event& event, int* currentScreen);
    void update(float deltaTime, sf::Vector2u windowSize);
    void draw(sf::RenderWindow& window);
    void setSaveSlot(int slot);
    void loadGame();
    void saveGame();
    int getScrap() const { return saveData.scrap; }
    SaveData* getSaveData() { return &saveData; }
    void setLevel(int level);
    void reset(); // Resetuje poziom do stanu początkowego

private:
    Background2 background_1;
    sf::Texture texture;
    sf::Sprite sprite;

    sf::CircleShape ball;
    sf::Texture playerTexture; // Tekstura dla kulki (seam.png)
    sf::Vector2f velocity;
    std::vector<Bullet2> bullets;
    sf::Vector2f bulletVelocity;
    std::vector<Enemy2> enemies;
    float enemySpawnTimer;
    std::mt19937 rng;
    std::uniform_real_distribution<float> distX;

    // System zapisu
    SaveData saveData;
    int currentSaveSlot;

    // System HP
    int maxHP = 10;
    int currentHP = 10;
    sf::CircleShape hpCircle; // Okrągły pasek HP wokół gracza
    std::optional<sf::Text> hpText;

    // Game Over
    bool isGameOver = false;
    sf::RectangleShape gameOverOverlay;
    std::optional<sf::Text> gameOverText;

    // UI złomu
    sf::Font font;
    std::optional<sf::Text> scrapText;

    // Konfiguracja przeciwników
    EnemyConfig enemyConfig;

    std::string getSaveFileName() const;
    void loadEnemyConfig(int level);
};

#endif // LEVEL2_H

