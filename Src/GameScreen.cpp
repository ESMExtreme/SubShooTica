#include "GameScreen.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>

GameScreen::GameScreen() : currentSaveSlot(0) {
    ball.setRadius(20.f);
    ball.setFillColor(sf::Color::Blue);
    ball.setPosition(sf::Vector2f(900.f, 900.f));
    velocity = sf::Vector2f(0.f, 0.f);
    bulletVelocity = sf::Vector2f(0.f, -300.f); // Strzelanie w górę
    enemySpawnTimer = 0.f;
    rng.seed(std::random_device{}());
    distX = std::uniform_real_distribution<float>(0.f, 1920.f); // Zakładając szerokość ekranu 1920

    // Inicjalizacja UI złomu
    if (!font.openFromFile("Assets/Fonts/BerlinSans.ttf")) {
        // handle error
    }
    scrapText.emplace(font);
    scrapText->setCharacterSize(30);
    scrapText->setFillColor(sf::Color::Yellow);
    scrapText->setPosition(sf::Vector2f(10.f, 10.f));
    scrapText->setString("Zlom: 0");
}

void GameScreen::handleEvent(const sf::Event& event, int* currentScreen) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        using Key = sf::Keyboard::Key;
        if (key->code == Key::Left) {
            velocity.x = -250.f;
        } else if (key->code == Key::Right) {
            velocity.x = 250.f;
        } else if (key->code == Key::Space) {
            // Strzelanie z dodaniem prędkości kulki
            Bullet bullet;
            bullet.shape.setRadius(5.f);
            bullet.shape.setFillColor(sf::Color::Red);
            bullet.shape.setPosition(sf::Vector2f(ball.getPosition().x + ball.getRadius(), ball.getPosition().y));
            bullet.velocity = bulletVelocity + sf::Vector2f(velocity.x * 0.02f, velocity.y);
            bullets.push_back(bullet);
        } else if (key->code == Key::Escape) {
            saveGame(); // Zapisz grę przed wyjściem
            *currentScreen = 3; // Zawsze wróć do GameMap (wybór poziomu)
        }
    } else if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>()) {
        if (keyReleased->code == sf::Keyboard::Key::Left || keyReleased->code == sf::Keyboard::Key::Right) {
            velocity.x = 0.f;
        } else if (keyReleased->code == sf::Keyboard::Key::Up || keyReleased->code == sf::Keyboard::Key::Down) {
            velocity.y = 0.f;
        }
    }
}

void GameScreen::update(float deltaTime, sf::Vector2u windowSize) {
    ball.move(velocity * deltaTime);

    // Aktualizacja pocisków
    for (auto& bullet : bullets) {
        bullet.shape.move(bullet.velocity * deltaTime);
        auto pos = bullet.shape.getPosition();
        float radius = bullet.shape.getRadius();
        // Odbicie od lewej ściany
        if (pos.x - radius < 0) {
            bullet.velocity.x = -bullet.velocity.x;
            bullet.shape.setPosition(sf::Vector2f(radius, pos.y));
        }
        // Odbicie od prawej ściany
        else if (pos.x + radius > windowSize.x) {
            bullet.velocity.x = -bullet.velocity.x;
            bullet.shape.setPosition(sf::Vector2f(windowSize.x - radius, pos.y));
        }
        // Nie odbijaj od góry, tylko usuń jeśli wyjdzie
    }
    // Usuwanie pocisków poza ekranem (góra i dół)
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) {
        return b.shape.getPosition().y < 0 || b.shape.getPosition().y > 1080; // Zakładając wysokość 1080, dostosuj jeśli trzeba
    }), bullets.end());

    // Pojawianie się wrogów
    enemySpawnTimer += deltaTime;
    if (enemySpawnTimer > 2.f) {
        enemySpawnTimer = 0.f;
        Enemy enemy;
        enemy.shape.setSize(sf::Vector2f(30.f, 30.f));
        enemy.shape.setFillColor(sf::Color::Green);
        float x = distX(rng);
        enemy.shape.setPosition(sf::Vector2f(x, 0.f));
        enemies.push_back(enemy);
    }

    // Aktualizacja wrogów
    for (auto& enemy : enemies) {
        sf::Vector2f direction = ball.getPosition() - enemy.shape.getPosition();
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) {
            direction /= length;
            enemy.velocity = direction * 100.f; // Prędkość wroga
        }
        enemy.shape.move(enemy.velocity * deltaTime);
    }

    // Kolizje między pociskami a wrogami
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
        bool bulletDestroyed = false;
        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
            if (enemyIt->shape.getGlobalBounds().contains(bulletIt->shape.getPosition())) {
                // Usuń wroga i pocisk
                enemyIt = enemies.erase(enemyIt);
                bulletDestroyed = true;
                // Dodaj złom za zabicie wroga
                saveData.scrap += 10;
                if (scrapText.has_value()) {
                    scrapText->setString("Zlom: " + std::to_string(saveData.scrap));
                }
                break;
            } else {
                ++enemyIt;
            }
        }
        if (bulletDestroyed) {
            bulletIt = bullets.erase(bulletIt);
        } else {
            ++bulletIt;
        }
    }

    // Usuwanie wrogów poza ekranem
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& e) {
        return e.shape.getPosition().y > 1080;
    }), enemies.end());
}

void GameScreen::draw(sf::RenderWindow& window) {
    window.draw(ball);
    for (const auto& bullet : bullets) {
        window.draw(bullet.shape);
    }
    for (const auto& enemy : enemies) {
        window.draw(enemy.shape);
    }
    // Rysuj UI ze złomem
    if (scrapText.has_value()) {
        window.draw(*scrapText);
    }
}

std::string GameScreen::getSaveFileName() const {
    return "Assets/save" + std::to_string(currentSaveSlot) + ".dat";
}

void GameScreen::setSaveSlot(int slot) {
    currentSaveSlot = slot;
    loadGame();
}

void GameScreen::loadGame() {
    std::string filename = getSaveFileName();
    if (SaveData::fileExists(filename)) {
        saveData.loadFromFile(filename);
        if (scrapText.has_value()) {
            scrapText->setString("Zlom: " + std::to_string(saveData.scrap));
        }
    } else {
        // Nowa gra - zresetuj dane
        saveData = SaveData();
        if (scrapText.has_value()) {
            scrapText->setString("Zlom: 0");
        }
    }
}

void GameScreen::saveGame() {
    std::string filename = getSaveFileName();
    saveData.saveToFile(filename);
}
