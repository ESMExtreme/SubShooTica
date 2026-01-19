#ifndef LEVEL2_H
#define LEVEL2_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <optional>
#include <memory>
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
    std::shared_ptr<sf::Texture> texture; // Shared_ptr aby tekstura żyła dopóki sprite używa
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
    void applyShopBonuses(); // Zastosuj bonusy ze sklepu

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
    sf::CircleShape hpInnerCircle; // Wewnętrzne wypełnione koło
    std::optional<sf::Text> hpText;
    std::optional<sf::Text> oxygenText; // Tekst O₂ w środku koła

    // Game Over
    bool isGameOver = false;
    sf::RectangleShape gameOverOverlay;
    std::optional<sf::Text> gameOverText;

    // Progress bar (pasek postępu)
    sf::Texture progressBarTexture;
    sf::Texture progressBarFillTexture;
    std::optional<sf::Sprite> progressBarBackground;
    std::optional<sf::Sprite> progressBarFill;
    std::optional<sf::Text> progressText;
    int totalEnemies = 0; // Łącznie wrogów do pokonania
    int enemiesKilled = 0; // Wrogów pokonanych
    bool levelCompleted = false; // Czy poziom ukończony
    sf::RectangleShape levelCompleteOverlay; // Zaciemnienie po wygranej
    std::optional<sf::Text> victoryText; // Tekst wygranej
    std::optional<sf::Text> pressEnterText; // Instrukcja "Naciśnij Enter"

    // UI złomu
    sf::Font font;
    std::optional<sf::Text> scrapText;

    // Konfiguracja przeciwników
    EnemyConfig enemyConfig;

    // Bonusy ze sklepu
    bool hasTripleShoot = false;
    int damageBonus = 0;
    float fireRateBonus = 0.0f; // Zmniejszenie cooldown'u
    int hpBonus = 0;

    std::string getSaveFileName() const;
    void loadEnemyConfig(int level);
};

#endif // LEVEL2_H

