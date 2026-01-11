#include "GameScreen.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>

GameScreen::GameScreen() : sprite(texture), currentSaveSlot(0) {

    // Inicjalizacja tła
    std::vector<std::string> backgroundFiles = {
    "Assets/Media/background1.png"
    };
    int bgIndex = 0;
    if (!background_1.texture.loadFromFile(backgroundFiles[bgIndex])) {
        // handle error
        std::cout << "Blad ladowania tekstury tla: " << backgroundFiles[bgIndex] << std::endl;
    } else {
        background_1.sprite.setTexture(background_1.texture);
    }
    // Make background scroll more visible
    background_1.scrollSpeed = 150.f;
    

    // Inicjalizacja piłki
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
     // Aktualizacja tła
    background_1.offsetY += background_1.scrollSpeed * deltaTime;
    if (background_1.offsetY >= background_1.getHeight()) {
        background_1.offsetY = 0.f;
    }

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
    sf::Vector2u windowSize = window.getSize();

    // Rysuj tło
    if (background_1.texture.getSize().x > 0 && background_1.texture.getSize().y > 0) {
        std::cout << "[DEBUG] draw: texSize=" << background_1.texture.getSize().x << "x" << background_1.texture.getSize().y
                  << " offsetY=" << background_1.offsetY << " scrollSpeed=" << background_1.scrollSpeed << std::endl;
        // Scale uniformly to match window width (preserve aspect ratio)
        float texW = static_cast<float>(background_1.texture.getSize().x);
        float texH = static_cast<float>(background_1.texture.getSize().y);
        float scale = static_cast<float>(windowSize.x) / texW;
        background_1.sprite.setScale(sf::Vector2f(scale, scale));

        // Rysuj pierwszą część tła (pozycja w pikselach tekstury przeskalowana przez 'scale')
        background_1.sprite.setPosition(sf::Vector2f(0.f, background_1.offsetY * scale));
        window.draw(background_1.sprite);

        // Rysuj drugą część dla płynnego przewijania
        if (background_1.offsetY > 0) {
            background_1.sprite.setPosition(sf::Vector2f(0.f, (background_1.offsetY - background_1.getHeight()) * scale));
            window.draw(background_1.sprite);
        }
    }
    

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

void GameScreen::setLevel(int level) {
    background_1.difficultyLevel = level;
    std::string bgFile = "Assets/Media/background" + std::to_string(level) + ".png";
    std::cout << "[DEBUG] setLevel: loading " << bgFile << std::endl;
    if (!background_1.texture.loadFromFile(bgFile)) {
        std::cout << "[DEBUG] Error loading background: " << bgFile << std::endl;
        // try default background1
        if (level != 1) {
            std::string fallback = "Assets/Media/background1.png";
            if (background_1.texture.loadFromFile(fallback)) {
                background_1.sprite.setTexture(background_1.texture);
                background_1.offsetY = 0.f;
                std::cout << "[DEBUG] Fallback background loaded: " << fallback << std::endl;
            }
        }
    } else {
        // std::cout << "[DEBUG] Background loaded, size: " << background_1.texture.getSize().x << "x" << background_1.texture.getSize().y << std::endl;
        // dodatkowa diagnostyka obrazu
        sf::Image img;
        if (img.loadFromFile(bgFile)) {
            const sf::Vector2u sz = img.getSize();
            uint64_t sum = 0;
            unsigned int minv = 255, maxv = 0;
            const unsigned char* px = img.getPixelsPtr();
            size_t count = sz.x * sz.y;
            for (size_t i = 0; i < count; ++i) {
                unsigned int r = px[i*4+0];
                unsigned int g = px[i*4+1];
                unsigned int b = px[i*4+2];
                unsigned int v = (r + g + b) / 3;
                sum += v;
                if (v < minv) minv = v;
                if (v > maxv) maxv = v;
            }
            double avg = static_cast<double>(sum) / static_cast<double>(count);
            std::cout << "[DEBUG] Image stats: avg=" << avg << " min=" << minv << " max=" << maxv << " (" << sz.x << "x" << sz.y << ")" << std::endl;
            // sample some pixels
            auto sample = [&](unsigned int sx, unsigned int sy){
                if (sx < sz.x && sy < sz.y) {
                    sf::Color c = img.getPixel(sf::Vector2u(sx, sy));
                    std::cout << "[DEBUG] sample("<<sx<<","<<sy<<")="<<(int)c.r<<","<<(int)c.g<<","<<(int)c.b<<"\n";
                }
            };
            sample(0,0);
            sample(sz.x/2, sz.y/4);
            sample(sz.x/2, sz.y/2);
            sample(sz.x/2, (sz.y*3)/4);
            sample(sz.x-1, sz.y-1);
        }
        background_1.sprite.setTexture(background_1.texture);
        background_1.sprite.setTextureRect(sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(static_cast<int>(background_1.texture.getSize().x), static_cast<int>(background_1.texture.getSize().y))));
        background_1.sprite.setOrigin(sf::Vector2f(0.f, 0.f));
        background_1.offsetY = 0.f;
    }
}
