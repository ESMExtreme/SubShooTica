 #ifndef LEVEL1_H
 #define LEVEL1_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <optional>
#include <memory>
#include "SaveData.h"
#include "EnemyConfig.h"

struct Background {
    sf::Texture texture;
    sf::Sprite sprite;
    float offsetY = 0.f;
    float scrollSpeed = 150.f;
    int difficultyLevel = 1;

    Background() : sprite(texture) {}
    float getHeight() const { return texture.getSize().y; }
};

struct Bullet {
    sf::CircleShape shape;
    sf::Vector2f velocity;
};

struct Enemy {
    sf::RectangleShape shape;
    std::optional<sf::Sprite> sprite; // Optional bo sprite wymaga tekstury
    std::shared_ptr<sf::Texture> texture; // Shared_ptr aby tekstura żyła dopóki sprite używa
    sf::Vector2f velocity;
    int hp = 1;
    bool useTexture = false; // true = sprite, false = shape
    int scrapReward = 10;
};

class Level1 {
public:
    Level1();
    ~Level1() = default;
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
    Background background_1;
    sf::Texture texture;
    sf::Sprite sprite;

    sf::CircleShape ball;
    sf::Texture playerTexture; // Tekstura dla kulki (seam.png)
    sf::Vector2f velocity;
    std::vector<Bullet> bullets;
    sf::Vector2f bulletVelocity;
    std::vector<Enemy> enemies;
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
    sf::CircleShape hpInnerCircle; // Wewnętrzne wypełnione koło
    std::optional<sf::Text> hpText;
    std::optional<sf::Text> oxygenText; // Tekst O₂ w środku koła

    // Game Over
    bool isGameOver = false;
    sf::RectangleShape gameOverOverlay;  // Zaciemnienie ekranu
    std::optional<sf::Text> gameOverText;

    // UI złomu
    sf::Font font;
    std::optional<sf::Text> scrapText;

    // Konfiguracja przeciwników
    EnemyConfig enemyConfig;

    std::string getSaveFileName() const;
    void loadEnemyConfig(int level);
};

#endif // LEVEL1_H
