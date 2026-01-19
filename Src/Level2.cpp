#include "Level2.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <map>

Level2::Level2() : sprite(texture), currentSaveSlot(0) {

    // Wczytaj konfigurację przeciwników dla poziomu 2
    loadEnemyConfig(2);

    // Inicjalizacja tła
    std::vector<std::string> backgroundFiles = {
    "Assets/Media/background2.png"
    };
    int bgIndex = 0;
    if (!background_1.texture.loadFromFile(backgroundFiles[bgIndex])) {
        // handle error
    } else {
        background_1.sprite.setTexture(background_1.texture);
    }
    background_1.scrollSpeed = 150.f;
    

    // Inicjalizacja piłki z teksturą seam.png
    ball.setRadius(70.f); // Zwiększony rozmiar (było 20.f)
    ball.setFillColor(sf::Color::White); // Białe wypełnienie (tekstura będzie widoczna)
    ball.setPosition(sf::Vector2f(900.f, 900.f));

    // Załaduj i nałóż teksturę seam.png na kulkę
    if (playerTexture.loadFromFile("Assets/Media/seam.png")) {
        ball.setTexture(&playerTexture);
    }

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

    // Inicjalizacja okrągłego paska HP
    hpCircle.setRadius(70.f); // Zmniejszony promień
    hpCircle.setPointCount(100); // Więcej punktów = płynniejszy okrąg
    hpCircle.setFillColor(sf::Color::Transparent);
    hpCircle.setOutlineThickness(12.f); // Pogrubiony (było 8)
    hpCircle.setOutlineColor(sf::Color::Green);
    hpCircle.setOrigin(sf::Vector2f(70.f, 70.f)); // Środek okręgu

    // Inicjalizacja wewnętrznego wypełnionego koła
    hpInnerCircle.setRadius(58.f); // Mniejszy odstęp
    hpInnerCircle.setPointCount(100);
    hpInnerCircle.setFillColor(sf::Color(101, 180, 179)); // RGB(101, 180, 179)
    hpInnerCircle.setOrigin(sf::Vector2f(58.f, 58.f)); // Środek okręgu

    // Inicjalizacja hpText (SFML 3 wymaga Font w konstruktorze)
    hpText.emplace(font, "10/10", 20);
    hpText->setFillColor(sf::Color::White);
    hpText->setPosition(sf::Vector2f(75.f, 190.f)); // Przesunięte niżej

    // Inicjalizacja tekstu "Zdrowie"
    oxygenText.emplace(font, "Zdrowie", 25); // Tekst "Zdrowie"
    oxygenText->setFillColor(sf::Color::White);
    oxygenText->setStyle(sf::Text::Bold);

    // Inicjalizacja ekranu Game Over
    gameOverOverlay.setSize(sf::Vector2f(1920.f, 1080.f));
    gameOverOverlay.setFillColor(sf::Color(0, 0, 0, 180));
    gameOverOverlay.setPosition(sf::Vector2f(0.f, 0.f));

    gameOverText.emplace(font, "GAME OVER\n\nNacisnij SPACJE aby wrocic do mapy", 60);
    gameOverText->setFillColor(sf::Color::Red);
    gameOverText->setPosition(sf::Vector2f(500.f, 400.f));

    // Inicjalizacja progress bar
    if (!progressBarTexture.loadFromFile("Assets/Media/progresbar.png")) {
        progressBarTexture.loadFromFile("Assets/Media/background1.png");
    }
    if (!progressBarFillTexture.loadFromFile("Assets/Media/progresbar2.png")) {
        progressBarFillTexture.loadFromFile("Assets/Media/background1.png");
    }

    progressBarBackground.emplace(progressBarTexture);
    progressBarBackground->setPosition(sf::Vector2f(760.f, 50.f));

    progressBarFill.emplace(progressBarFillTexture);
    progressBarFill->setPosition(sf::Vector2f(760.f, 50.f));
    progressBarFill->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(0, static_cast<int>(progressBarFillTexture.getSize().y))));

    progressText.emplace(font, "Poziom 2", 25);
    progressText->setFillColor(sf::Color::White);
    progressText->setStyle(sf::Text::Bold);
    progressText->setPosition(sf::Vector2f(910.f, 60.f));

    // Inicjalizacja ekranu zwycięstwa
    levelCompleteOverlay.setSize(sf::Vector2f(1920.f, 1080.f));
    levelCompleteOverlay.setFillColor(sf::Color(0, 0, 0, 150));
    levelCompleteOverlay.setPosition(sf::Vector2f(0.f, 0.f));

    victoryText.emplace(font, "Poziom Ukoczony!", 80);
    victoryText->setFillColor(sf::Color::Green);
    victoryText->setStyle(sf::Text::Bold);
    sf::FloatRect victoryBounds = victoryText->getLocalBounds();
    victoryText->setOrigin(sf::Vector2f(victoryBounds.size.x / 2.f, victoryBounds.size.y / 2.f));
    victoryText->setPosition(sf::Vector2f(960.f, 480.f));

    pressEnterText.emplace(font, "Nacisnij Enter aby kontynuowac", 40);
    pressEnterText->setFillColor(sf::Color::White);
    pressEnterText->setStyle(sf::Text::Bold);
    sf::FloatRect enterBounds = pressEnterText->getLocalBounds();
    pressEnterText->setOrigin(sf::Vector2f(enterBounds.size.x / 2.f, enterBounds.size.y / 2.f));
    pressEnterText->setPosition(sf::Vector2f(960.f, 600.f));
}

void Level2::handleEvent(const sf::Event& event, int* currentScreen) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        using Key = sf::Keyboard::Key;

        // Obsługa ukończenia poziomu - powrót do mapy po Enter
        if (levelCompleted && key->code == Key::Enter) {
            saveGame();
            reset();
            *currentScreen = 3; // Wróć do mapy
            return;
        }

        // Jeśli poziom ukończony, ignoruj inne klawisze
        if (levelCompleted) {
            return;
        }

        // Obsługa Game Over - powrót do mapy po spacji
        if (isGameOver && key->code == Key::Space) {
            // Reset stanu gry
            currentHP = maxHP;
            isGameOver = false;
            enemies.clear();
            bullets.clear();
            ball.setPosition(sf::Vector2f(900.f, 900.f));
            saveGame(); // Zapisz stan przed wyjściem
            *currentScreen = 3; // Wróć do mapy
            return;
        }

        // Jeśli gra się skończyła, ignoruj inne klawisze
        if (isGameOver) {
            return;
        }

        if (key->code == Key::Left) {
            velocity.x = -250.f;
        }
        if (key->code == Key::Right) {
            velocity.x = 250.f;
        }
        if (key->code == Key::Up) {
            velocity.y = -250.f;
        }
        if (key->code == Key::Down) {
            velocity.y = 250.f;
        }
        // Strzelanie obsługiwane w update() przez isKeyPressed()
        if (key->code == Key::Escape) {
            saveGame(); // Zapisz grę przed wyjściem
            reset(); // Resetuj poziom przed wyjściem
            *currentScreen = 3; // Zawsze wróć do GameMap (wybór poziomu)
        }
    }
    if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>()) {
        if (keyReleased->code == sf::Keyboard::Key::Left || keyReleased->code == sf::Keyboard::Key::Right) {
            velocity.x = 0.f;
        }
        if (keyReleased->code == sf::Keyboard::Key::Up || keyReleased->code == sf::Keyboard::Key::Down) {
            velocity.y = 0.f;
        }
    }
}

void Level2::update(float deltaTime, sf::Vector2u windowSize) {
    // Jeśli poziom ukończony lub Game Over, nie aktualizuj gry
    if (isGameOver || levelCompleted) {
        return;
    }

     // Aktualizacja tła
    background_1.offsetY += background_1.scrollSpeed * deltaTime;
    if (background_1.offsetY >= background_1.getHeight()) {
        background_1.offsetY = 0.f;
    }

    // Automatyczne strzelanie gdy SPACE jest trzymany
    static float shootCooldown = 0.f;
    shootCooldown -= deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && shootCooldown <= 0.f) {
        // Stwórz pocisk
        float playerVelX = velocity.x;

        // Główny pocisk
        Bullet2 bullet2;
        bullet2.shape.setRadius(5.f);
        bullet2.shape.setFillColor(sf::Color::Red);
        bullet2.shape.setPosition(sf::Vector2f(ball.getPosition().x + ball.getRadius(), ball.getPosition().y));
        bullet2.velocity = sf::Vector2f(playerVelX * 0.02f, -300.f);
        bullets.push_back(bullet2);

        // Triple shoot - dodaj pociski w kierunkach bocznych
        if (hasTripleShoot) {
            // Pocisk w lewo
            Bullet2 bulletLeft;
            bulletLeft.shape.setRadius(5.f);
            bulletLeft.shape.setFillColor(sf::Color::Red);
            bulletLeft.shape.setPosition(sf::Vector2f(ball.getPosition().x, ball.getPosition().y));
            bulletLeft.velocity = sf::Vector2f(-100.f, -300.f);
            bullets.push_back(bulletLeft);

            // Pocisk w prawo
            Bullet2 bulletRight;
            bulletRight.shape.setRadius(5.f);
            bulletRight.shape.setFillColor(sf::Color::Red);
            bulletRight.shape.setPosition(sf::Vector2f(ball.getPosition().x + ball.getRadius() + 20.f, ball.getPosition().y));
            bulletRight.velocity = sf::Vector2f(100.f, -300.f);
            bullets.push_back(bulletRight);
        }

        // Stosuj fire rate bonus - zmniejsz cooldown
        float baseCooldown = 0.45f;
        shootCooldown = baseCooldown - fireRateBonus;
    }

    ball.move(velocity * deltaTime);

    // Ograniczenia pozycji kulki - nie może wyjść poza ekran
    sf::Vector2f ballPos = ball.getPosition();
    float ballRadius = ball.getRadius();

    // Ograniczenie lewo (przesunięte o 20px więcej w lewo)
    if (ballPos.x - ballRadius < -50.f) {
        ball.setPosition(sf::Vector2f(ballRadius - 50.f, ballPos.y));
        velocity.x = 0.f; // Zatrzymaj ruch w lewo
    }
    // Ograniczenie prawo
    if (ballPos.x + ballRadius > static_cast<float>(windowSize.x)) {
        ball.setPosition(sf::Vector2f(static_cast<float>(windowSize.x) - ballRadius, ballPos.y));
        velocity.x = 0.f; // Zatrzymaj ruch w prawo
    }

    // Ograniczenie góra
    if (ballPos.y - ballRadius < 0.f) {
        ball.setPosition(sf::Vector2f(ballPos.x, ballRadius));
        velocity.y = 0.f; // Zatrzymaj ruch w górę
    }
    // Ograniczenie dół
    if (ballPos.y + ballRadius > static_cast<float>(windowSize.y)) {
        ball.setPosition(sf::Vector2f(ballPos.x, static_cast<float>(windowSize.y) - ballRadius));
        velocity.y = 0.f; // Zatrzymaj ruch w dół
    }

    // Aktualizacja pocisków
    for (auto& bullet2 : bullets) {
        bullet2.shape.move(bullet2.velocity * deltaTime);
        auto pos = bullet2.shape.getPosition();
        float radius = bullet2.shape.getRadius();
        // Odbicie od lewej ściany
        if (pos.x - radius < 0) {
            bullet2.velocity.x = -bullet2.velocity.x;
            bullet2.shape.setPosition(sf::Vector2f(radius, pos.y));
        }
        // Odbicie od prawej ściany
        else if (pos.x + radius > windowSize.x) {
            bullet2.velocity.x = -bullet2.velocity.x;
            bullet2.shape.setPosition(sf::Vector2f(windowSize.x - radius, pos.y));
        }
        // Nie odbijaj od góry, tylko usuń jeśli wyjdzie
    }
    // Usuwanie pocisków poza ekranem (góra i dół)
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet2& b) {
        return b.shape.getPosition().y < 0 || b.shape.getPosition().y > 1080; // Zakładając wysokość 1080, dostosuj jeśli trzeba
    }), bullets.end());

    // Pojawianie się wrogów
    enemySpawnTimer += deltaTime;
    if (enemySpawnTimer > enemyConfig.spawnInterval && enemies.size() < static_cast<size_t>(enemyConfig.maxEnemies)) {
        enemySpawnTimer = 0.f;

        // Jeśli są zdefiniowane typy przeciwników, wybierz losowo
        if (!enemyConfig.enemyTypes.empty()) {
            // Oblicz całkowitą wagę
            int totalWeight = 0;
            for (const auto& type : enemyConfig.enemyTypes) {
                totalWeight += type.spawnWeight;
            }

            // Losuj typ na podstawie wag
            std::uniform_int_distribution<int> weightDist(0, totalWeight - 1);
            int randomWeight = weightDist(rng);

            int currentWeight = 0;
            const EnemyTypeConfig* selectedType = nullptr;
            for (const auto& type : enemyConfig.enemyTypes) {
                currentWeight += type.spawnWeight;
                if (randomWeight < currentWeight) {
                    selectedType = &type;
                    break;
                }
            }

            if (selectedType) {
                Enemy2 enemy;
                float x = distX(rng);

                if (selectedType->type == EnemyType::Texture) {
                    // Wróg z teksturą
                    enemy.texture = std::make_shared<sf::Texture>();
                    if (enemy.texture->loadFromFile(selectedType->texturePath)) {
                        enemy.useTexture = true;
                        enemy.sprite.emplace(*enemy.texture);
                        enemy.sprite->setScale(sf::Vector2f(
                            selectedType->sizeX / enemy.texture->getSize().x,
                            selectedType->sizeY / enemy.texture->getSize().y
                        ));
                        enemy.sprite->setPosition(sf::Vector2f(x, 0.f));
                        enemy.hp = selectedType->hp;
                        enemy.shape.setSize(sf::Vector2f(selectedType->sizeX, selectedType->sizeY));
                    } else {
                        // Fallback na shape jeśli nie można załadować tekstury
                        enemy.useTexture = false;
                        enemy.shape.setSize(sf::Vector2f(selectedType->sizeX, selectedType->sizeY));
                        enemy.shape.setFillColor(sf::Color(selectedType->colorR, selectedType->colorG, selectedType->colorB));
                        enemy.shape.setOrigin(sf::Vector2f(selectedType->sizeX / 2.f, selectedType->sizeY / 2.f));
                        enemy.shape.setPosition(sf::Vector2f(x, 0.f));
                        enemy.hp = selectedType->hp;
                    }
                } else {
                    // Wróg z shape'em (prostokątem)
                    enemy.useTexture = false;
                    enemy.shape.setSize(sf::Vector2f(selectedType->sizeX, selectedType->sizeY));
                    enemy.shape.setFillColor(sf::Color(selectedType->colorR, selectedType->colorG, selectedType->colorB));
                    enemy.shape.setOrigin(sf::Vector2f(selectedType->sizeX / 2.f, selectedType->sizeY / 2.f));
                    enemy.shape.setPosition(sf::Vector2f(x, 0.f));
                    enemy.hp = selectedType->hp;
                }

                enemies.push_back(enemy);
            }
        } else {
            // Fallback na stary system jeśli brak typów
            Enemy2 enemy;
            enemy.shape.setSize(sf::Vector2f(enemyConfig.sizeX, enemyConfig.sizeY));
            enemy.shape.setFillColor(sf::Color(enemyConfig.colorR, enemyConfig.colorG, enemyConfig.colorB));
            enemy.shape.setOrigin(sf::Vector2f(
                enemyConfig.sizeX / 2.f,
                enemyConfig.sizeY / 2.f
            ));
            enemy.hp = enemyConfig.hp;
            float x = distX(rng);
            enemy.shape.setPosition(sf::Vector2f(x, 0.f));
            enemies.push_back(enemy);
        }
    }

    // Aktualizacja wrogów
    for (auto& enemy2 : enemies) {
        sf::Vector2f direction = ball.getPosition() - enemy2.shape.getPosition();

        // Dla sprite'ów użyj innej pozycji
        if (enemy2.useTexture && enemy2.sprite.has_value()) {
            direction = ball.getPosition() - enemy2.sprite->getPosition();
        }

        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) {
            direction /= length;
            enemy2.velocity = direction * 100.f; // Prędkość wroga

            // Oblicz kąt do gracza (w stopniach)
            float angleRadians = std::atan2(direction.y, direction.x);
            float angleDegrees = angleRadians * 180.f / 3.14159265f;
            // Odejmij 90 stopni, żeby przeciwnicy szli przodem (nie tyłem)
            angleDegrees -= 90.f;

            // Ustaw rotację
            if (enemy2.useTexture && enemy2.sprite.has_value()) {
                enemy2.sprite->setRotation(sf::degrees(angleDegrees));
            } else {
                enemy2.shape.setRotation(sf::degrees(angleDegrees));
            }
        }

        // Rusz wroga
        if (enemy2.useTexture && enemy2.sprite.has_value()) {
            enemy2.sprite->move(enemy2.velocity * deltaTime);
        } else {
            enemy2.shape.move(enemy2.velocity * deltaTime);
        }
    }

    // Kolizje między pociskami a wrogami
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
        bool bulletDestroyed = false;
        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
            sf::FloatRect enemyBounds = (enemyIt->useTexture && enemyIt->sprite.has_value()) ?
                enemyIt->sprite->getGlobalBounds() :
                enemyIt->shape.getGlobalBounds();

            if (enemyBounds.contains(bulletIt->shape.getPosition())) {
                // Zmniejsz HP wroga + dodaj damageBoost
                enemyIt->hp -= (1 + damageBonus);
                bulletDestroyed = true;

                // Usuń wroga jeśli HP <= 0
                if (enemyIt->hp <= 0) {
                    // Dodaj złom za zabicie wroga (z konfiguracji)
                    saveData.scrap += enemyConfig.scrapReward;
                    if (scrapText.has_value()) {
                        scrapText->setString("Zlom: " + std::to_string(saveData.scrap));
                    }

                    // Zwiększ licznik zabitych i aktualizuj progress bar
                    enemiesKilled++;

                    // Aktualizuj progress bar
                    if (totalEnemies > 0 && progressBarFill.has_value()) {
                        float progressPercentage = static_cast<float>(enemiesKilled) / static_cast<float>(totalEnemies);
                        int fillWidth = static_cast<int>(progressBarFillTexture.getSize().x * progressPercentage);
                        progressBarFill->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(fillWidth, static_cast<int>(progressBarFillTexture.getSize().y))));
                    }

                    // Sprawdź czy poziom ukończony
                    if (enemiesKilled >= totalEnemies && totalEnemies > 0) {
                        levelCompleted = true;
                    }

                    enemyIt = enemies.erase(enemyIt);
                } else {
                    ++enemyIt;
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

    // Kolizje między graczem a wrogami
    sf::FloatRect playerBounds = ball.getGlobalBounds();
    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
        sf::FloatRect enemyBounds = (enemyIt->useTexture && enemyIt->sprite.has_value()) ?
            enemyIt->sprite->getGlobalBounds() :
            enemyIt->shape.getGlobalBounds();

        if (playerBounds.findIntersection(enemyBounds).has_value()) {
            // Gracz otrzymał obrażenia
            currentHP--;

            // Aktualizuj okrągły pasek HP (zmień kolor w zależności od HP)
            if (currentHP > 6) {
                hpCircle.setOutlineColor(sf::Color::Green);
            } else if (currentHP > 3) {
                hpCircle.setOutlineColor(sf::Color::Yellow);
            } else {
                hpCircle.setOutlineColor(sf::Color::Red);
            }

            hpText->setString(std::to_string(currentHP) + "/" + std::to_string(maxHP));

            // Usuń wroga po kolizji
            enemyIt = enemies.erase(enemyIt);

            // Jeśli HP spadło do 0 lub poniżej, koniec gry
            if (currentHP <= 0) {
                currentHP = 0;
                isGameOver = true;  // Aktywuj ekran Game Over
            }
        } else {
            ++enemyIt;
        }
    }

    // Usuwanie wrogów poza ekranem
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Enemy2& e) {
        if (e.useTexture && e.sprite.has_value()) {
            return e.sprite->getPosition().y > 1080;
        } else {
            return e.shape.getPosition().y > 1080;
        }
    }), enemies.end());
}

void Level2::draw(sf::RenderWindow& window) {
    sf::Vector2u windowSize = window.getSize();

    // Rysuj tło
    if (background_1.texture.getSize().x > 0 && background_1.texture.getSize().y > 0) {
        float texW = static_cast<float>(background_1.texture.getSize().x);
        float texH = static_cast<float>(background_1.texture.getSize().y);
        float scale = static_cast<float>(windowSize.x) / texW;
        background_1.sprite.setScale(sf::Vector2f(scale, scale));

        background_1.sprite.setPosition(sf::Vector2f(0.f, background_1.offsetY * scale));
        window.draw(background_1.sprite);

        if (background_1.offsetY > 0) {
            background_1.sprite.setPosition(sf::Vector2f(0.f, (background_1.offsetY - background_1.getHeight()) * scale));
            window.draw(background_1.sprite);
        }
    }
    

    window.draw(ball);
    for (const auto& bullet2 : bullets) {
        window.draw(bullet2.shape);
    }
    for (const auto& enemy2 : enemies) {
        if (enemy2.useTexture && enemy2.sprite.has_value()) {
            window.draw(*enemy2.sprite);
        } else {
            window.draw(enemy2.shape);
        }
    }
    // Rysuj UI ze złomem
    if (scrapText.has_value()) {
        window.draw(*scrapText);
    }

    // Rysuj okrągły pasek HP w stałej pozycji (lewy górny róg)
    sf::Vector2f hpPosition(100.f, 200.f); // Przesunięte: 50px w lewo (było 150), 100px w dół (było 100)

    // Rysuj wewnętrzne wypełnione koło (RGB 101, 180, 179)
    hpInnerCircle.setPosition(hpPosition);
    window.draw(hpInnerCircle);

    // Rysuj pełny okrąg jako tło (szary) - dopasowany do nowych rozmiarów
    sf::CircleShape hpBackground;
    hpBackground.setRadius(70.f); // Dopasowany promień
    hpBackground.setPointCount(100);
    hpBackground.setFillColor(sf::Color::Transparent);
    hpBackground.setOutlineThickness(12.f); // Pogrubiony (było 8)
    hpBackground.setOutlineColor(sf::Color(80, 80, 80, 150)); // Szare tło
    hpBackground.setOrigin(sf::Vector2f(70.f, 70.f));
    hpBackground.setPosition(hpPosition);
    window.draw(hpBackground);

    // Rysuj łuk HP (tylko część odpowiadającą aktualnemu HP)
    float hpPercentage = static_cast<float>(currentHP) / static_cast<float>(maxHP);

    // Używamy VertexArray do narysowania łuku
    const int segments = static_cast<int>(100 * hpPercentage); // Liczba segmentów zależna od HP
    sf::VertexArray hpArc(sf::PrimitiveType::TriangleStrip);

    float radius = 70.f; // Dopasowany promień
    float thickness = 12.f; // Pogrubiony (było 8)
    float angleRange = 360.f * hpPercentage; // Kąt łuku w stopniach

    sf::Color hpColor;
    if (currentHP > 6) {
        hpColor = sf::Color::Green;
    } else if (currentHP > 3) {
        hpColor = sf::Color::Yellow;
    } else {
        hpColor = sf::Color::Red;
    }

    // Rysuj łuk od góry (270°) w prawo - teraz w stałej pozycji
    for (int i = 0; i <= segments; ++i) {
        float angle = (270.f - angleRange / 2.f + (angleRange * i / 100.f)) * 3.14159f / 180.f;

        float x1 = hpPosition.x + std::cos(angle) * (radius - thickness / 2.f);
        float y1 = hpPosition.y + std::sin(angle) * (radius - thickness / 2.f);
        float x2 = hpPosition.x + std::cos(angle) * (radius + thickness / 2.f);
        float y2 = hpPosition.y + std::sin(angle) * (radius + thickness / 2.f);

        sf::Vertex v1;
        v1.position = sf::Vector2f(x1, y1);
        v1.color = hpColor;
        hpArc.append(v1);

        sf::Vertex v2;
        v2.position = sf::Vector2f(x2, y2);
        v2.color = hpColor;
        hpArc.append(v2);
    }

    window.draw(hpArc);

    // Rysuj tekst "Zdrowie" w środku koła - przesunięty wyżej
    if (oxygenText.has_value()) {
        // Wyśrodkuj tekst
        sf::FloatRect textBounds = oxygenText->getLocalBounds();
        oxygenText->setOrigin(sf::Vector2f(textBounds.size.x / 2.f, textBounds.size.y / 2.f));
        // Przesunięty wyżej - nad tekstem HP
        oxygenText->setPosition(sf::Vector2f(hpPosition.x, hpPosition.y - 25.f));
        window.draw(*oxygenText);
    }

    if (hpText.has_value()) {
        window.draw(*hpText);
    }

    // Rysuj pasek postępu
    if (progressBarFill.has_value()) {
        window.draw(*progressBarFill);
    }
    if (progressBarBackground.has_value()) {
        window.draw(*progressBarBackground);
    }
    if (progressText.has_value()) {
        window.draw(*progressText);
    }

    // Rysuj ekran wygranej jeśli poziom ukończony
    if (levelCompleted) {
        window.draw(levelCompleteOverlay);
        if (victoryText.has_value()) {
            window.draw(*victoryText);
        }
        if (pressEnterText.has_value()) {
            window.draw(*pressEnterText);
        }
    }

    // Rysuj ekran Game Over jeśli gra się skończyła
    if (isGameOver && gameOverText.has_value()) {
        window.draw(gameOverOverlay);
        window.draw(*gameOverText);
    }
}

std::string Level2::getSaveFileName() const {
    return "Assets/save" + std::to_string(currentSaveSlot) + ".json";
}

void Level2::setSaveSlot(int slot) {
    currentSaveSlot = slot;
    loadGame();
}

void Level2::loadGame() {
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

void Level2::saveGame() {
    std::string filename = getSaveFileName();
    saveData.saveToFile(filename);
}

void Level2::setLevel(int level) {
    background_1.difficultyLevel = level;
    std::string bgFile = "Assets/Media/background2.png";
    if (!background_1.texture.loadFromFile(bgFile)) {
        // Spróbuj załadować domyślne tło
        if (level != 2) {
            std::string fallback = "Assets/Media/background2.png";
            if (background_1.texture.loadFromFile(fallback)) {
                background_1.sprite.setTexture(background_1.texture);
                background_1.offsetY = 0.f;
            }
        }
    } else {
        background_1.sprite.setTexture(background_1.texture);
        background_1.sprite.setTextureRect(sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(static_cast<int>(background_1.texture.getSize().x), static_cast<int>(background_1.texture.getSize().y))));
        background_1.sprite.setOrigin(sf::Vector2f(0.f, 0.f));
        background_1.offsetY = 0.f;
    }

    // Wczytaj konfigurację przeciwników dla tego poziomu
    loadEnemyConfig(level);
}

void Level2::loadEnemyConfig(int level) {
    std::string configFile = "Assets/level" + std::to_string(level) + "_enemies.txt";
    std::ifstream file(configFile);

    if (!file.is_open()) {
        // Jeśli plik nie istnieje, użyj domyślnych wartości
        return;
    }

    totalEnemies = 0; // Reset licznika
    enemiesKilled = 0; // Reset zabitych
    enemyConfig.enemyTypes.clear();
    std::map<int, EnemyTypeConfig> tempEnemyTypes;

    std::string line;
    while (std::getline(file, line)) {
        // Pomiń puste linie i komentarze
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Parsuj linię: parametr:wartość
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) {
            continue;
        }

        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);

        // Wczytaj wartości
        if (key == "spawn_interval") {
            enemyConfig.spawnInterval = std::stof(value);
        } else if (key == "max_enemies") {
            enemyConfig.maxEnemies = std::stoi(value);
            totalEnemies = enemyConfig.maxEnemies; // Ustaw całkowitą liczbę wrogów
        } else if (key.find("enemy") == 0) {
            // Obsługa nowych typów enemyów: enemy1_type, enemy2_hp, itd.
            size_t underscorePos = key.find('_');
            if (underscorePos != std::string::npos) {
                int enemyIndex = std::stoi(key.substr(5, underscorePos - 5));
                std::string param = key.substr(underscorePos + 1);

                if (param == "type") {
                    tempEnemyTypes[enemyIndex].type = (value == "texture") ? EnemyType::Texture : EnemyType::Shape;
                } else if (param == "texture") {
                    tempEnemyTypes[enemyIndex].texturePath = value;
                } else if (param == "hp") {
                    tempEnemyTypes[enemyIndex].hp = std::stoi(value);
                } else if (param == "speed") {
                    tempEnemyTypes[enemyIndex].speed = std::stof(value);
                } else if (param == "size_x") {
                    tempEnemyTypes[enemyIndex].sizeX = std::stof(value);
                } else if (param == "size_y") {
                    tempEnemyTypes[enemyIndex].sizeY = std::stof(value);
                } else if (param == "color_r") {
                    tempEnemyTypes[enemyIndex].colorR = std::stoi(value);
                } else if (param == "color_g") {
                    tempEnemyTypes[enemyIndex].colorG = std::stoi(value);
                } else if (param == "color_b") {
                    tempEnemyTypes[enemyIndex].colorB = std::stoi(value);
                } else if (param == "scrap_reward") {
                    tempEnemyTypes[enemyIndex].scrapReward = std::stoi(value);
                } else if (param == "spawn_weight") {
                    tempEnemyTypes[enemyIndex].spawnWeight = std::stoi(value);
                }
            }
        } else if (key == "enemy_hp") {
            // Dla kompatybilności wstecznej
            enemyConfig.hp = std::stoi(value);
        } else if (key == "enemy_speed") {
            enemyConfig.speed = std::stof(value);
        } else if (key == "enemy_size_x") {
            enemyConfig.sizeX = std::stof(value);
        } else if (key == "enemy_size_y") {
            enemyConfig.sizeY = std::stof(value);
        } else if (key == "enemy_color_r") {
            enemyConfig.colorR = std::stoi(value);
        } else if (key == "enemy_color_g") {
            enemyConfig.colorG = std::stoi(value);
        } else if (key == "enemy_color_b") {
            enemyConfig.colorB = std::stoi(value);
        } else if (key == "scrap_reward") {
            enemyConfig.scrapReward = std::stoi(value);
        }
    }

    // Przenieś zmapowane typy do vectora
    for (auto& pair : tempEnemyTypes) {
        enemyConfig.enemyTypes.push_back(pair.second);
    }

    file.close();
}

void Level2::reset() {
    // Reset HP
    currentHP = maxHP;
    isGameOver = false;
    levelCompleted = false;

    // Wyczyść przeciwników i pociski
    enemies.clear();
    bullets.clear();

    // Resetuj pozycję gracza
    ball.setPosition(sf::Vector2f(900.f, 900.f));
    velocity = sf::Vector2f(0.f, 0.f);

    // Resetuj timer spawnu przeciwników
    enemySpawnTimer = 0.f;

    // Reset tła
    background_1.offsetY = 0.f;

    // Reset progress bar
    enemiesKilled = 0;
    if (progressBarFill.has_value()) {
        progressBarFill->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(0, static_cast<int>(progressBarFillTexture.getSize().y))));
    }

    // Aktualizuj okrągły pasek HP (zresetuj kolor na zielony)
    hpCircle.setOutlineColor(sf::Color::Green);

    if (hpText.has_value()) {
        hpText->setString(std::to_string(currentHP) + "/" + std::to_string(maxHP));
    }
}

void Level2::applyShopBonuses() {
    // Stosuj bonusy ze sklepu do lokalnych zmiennych
    hasTripleShoot = saveData.tripleShoot;
    damageBonus = saveData.damageBoost;
    fireRateBonus = saveData.fireRateBonus;
    hpBonus = saveData.hpBonus;

    // Zwiększ maksymalne HP
    maxHP = 10 + hpBonus;
    currentHP = std::min(currentHP + hpBonus, maxHP);
}

