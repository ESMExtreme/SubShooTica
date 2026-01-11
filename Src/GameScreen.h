 #ifndef GAMESCREEN_H
 #define GAMESCREEN_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <optional>
#include "SaveData.h"

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
    sf::Vector2f velocity;
};

class GameScreen {
public:
    GameScreen();
    ~GameScreen() = default;
    void handleEvent(const sf::Event& event, int* currentScreen);
    void update(float deltaTime, sf::Vector2u windowSize);
    void draw(sf::RenderWindow& window);
    void setSaveSlot(int slot);
    void loadGame();
    void saveGame();
    int getScrap() const { return saveData.scrap; }
    void setLevel(int level);

private:
    Background background_1;
    sf::Texture texture;
    sf::Sprite sprite;

    sf::CircleShape ball;
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

    // UI złomu
    sf::Font font;
    std::optional<sf::Text> scrapText;

    std::string getSaveFileName() const;
};

#endif // GAMESCREEN_H