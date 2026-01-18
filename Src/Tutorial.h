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

    // Nieruchomy przeciwnik (do kolizji)
    struct TutorialEnemy {
        sf::RectangleShape shape;
        sf::Vector2f fixedPositionOnBackground; // Stała pozycja na TLE
        bool isActive = true; // Czy przeciwnik jest widoczny i aktywny
    };
    TutorialEnemy tutorialEnemy;

    // Faza tutorialu: 1 = początek, 2 = po wjechaniu w pierwszego przeciwnika
    int tutorialPhase = 1;

    // Dwóch przeciwników do zestrzelenia (faza 2)
    struct ShootableEnemy {
        sf::RectangleShape shape;
        sf::Vector2f position;
        int hp = 3; // 3 HP - trzeba trafić 3 razy
        bool isActive = true;
    };
    ShootableEnemy shootableEnemy1;
    ShootableEnemy shootableEnemy2;

    // Pasek postępu (pomarańczowy)
    int totalEnemies = 2; // Łącznie przeciwników w tutorialu
    int enemiesKilled = 0; // Ilu przeciwników zabito
    sf::RectangleShape progressBarBackground;
    sf::RectangleShape progressBarFill;
    std::optional<sf::Text> progressText;

    // Napis "Wygrałeś"
    bool levelCompleted = false;
    std::optional<sf::Text> victoryText;
    std::optional<sf::Text> pressEnterText; // Instrukcja "Naciśnij Enter"

    // Przyciemnienie ekranu po wygranej
    sf::RectangleShape darkenOverlay;

    // Strzałka wskazująca na przeciwnika
    sf::Texture enemyArrowTexture;
    std::optional<sf::Sprite> enemyArrowSprite;
    std::optional<sf::Text> enemyInstructionText;

    // Obrazek Spacebar i instrukcja strzelania
    sf::Texture spacebarTexture;
    std::optional<sf::Sprite> spacebarSprite;
    sf::Texture spacebarArrowTexture;
    std::optional<sf::Sprite> spacebarArrowSprite;
    std::optional<sf::Text> spacebarText;

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

    // 3 strzałki arrow.png z tekstami
    sf::Texture arrowTexture1;
    sf::Texture arrowTexture2;
    sf::Texture arrowTexture3;
    std::optional<sf::Sprite> arrowSprite1;
    std::optional<sf::Sprite> arrowSprite2;
    std::optional<sf::Sprite> arrowSprite3;
    std::optional<sf::Text> arrowText1;
    std::optional<sf::Text> arrowText2;
    std::optional<sf::Text> arrowText3;

    std::string getSaveFileName() const;
};

#endif // TUTORIAL_H
