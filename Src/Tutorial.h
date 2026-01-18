#ifndef TUTORIAL_H
#define TUTORIAL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "SaveData.h"

struct TutorialBackground {
    sf::Texture texture;
    std::optional<sf::Sprite> sprite;
    float offsetY = 0.f;
    float scrollSpeed = 100.f; // Wolniejsze scrollowanie w tutorialu

    float getHeight() const { return static_cast<float>(texture.getSize().y); }
};

struct TutorialBullet {
    sf::CircleShape shape;
    sf::Vector2f velocity;
};

class Tutorial {
public:
    Tutorial();
    ~Tutorial() = default;
    void handleEvent(const sf::Event& event, int* currentScreen);
    void update(float deltaTime, sf::Vector2u windowSize);
    void draw(sf::RenderWindow& window);
    void setSaveSlot(int slot);
    void reset(); // Resetuje tutorial do stanu początkowego

private:
    TutorialBackground background;
    sf::Texture playerTexture;
    sf::CircleShape ball;
    sf::Vector2f velocity;
    std::vector<TutorialBullet> bullets;
    sf::Vector2f bulletVelocity;

    // System zapisu
    SaveData saveData;
    int currentSaveSlot;

    // System HP
    int maxHP = 10;
    int currentHP = 10;
    sf::CircleShape hpCircle;
    sf::CircleShape hpInnerCircle;
    std::optional<sf::Text> hpText;
    std::optional<sf::Text> healthText; // Tekst "Zdrowie"

    // UI
    sf::Font font;
    std::optional<sf::Text> scrapText;

    // Instrukcje tutorialu
    std::optional<sf::Text> instructionText;
    sf::Texture arrowsTexture;
    std::optional<sf::Sprite> arrowsSprite;

    std::string getSaveFileName() const;
};

#endif // TUTORIAL_H
