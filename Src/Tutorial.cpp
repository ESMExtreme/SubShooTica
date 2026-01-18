#include "Tutorial.h"
#include <cmath>
#include <algorithm>

Tutorial::Tutorial() : currentSaveSlot(0) {
    // Załaduj tło tutorialu
    if (!background.texture.loadFromFile("Assets/Media/tutorial/background0.png")) {
        // Fallback do domyślnego tła
        background.texture.loadFromFile("Assets/Media/background1.png");
    }
    background.sprite.emplace(background.texture);
    background.scrollSpeed = 100.f;

    // Tło statyczne - brak przewijania
    float backgroundHeight = static_cast<float>(background.texture.getSize().y);

    // Inicjalizacja piłki z teksturą seam.png
    ball.setRadius(70.f);
    ball.setFillColor(sf::Color::White);
    // Gracz zaczyna na środku ekranu
    ball.setPosition(sf::Vector2f(960.f, 540.f));

    if (playerTexture.loadFromFile("Assets/Media/seam.png")) {
        ball.setTexture(&playerTexture);
    }

    velocity = sf::Vector2f(0.f, 0.f);
    bulletVelocity = sf::Vector2f(0.f, -300.f);

    // Inicjalizacja nieruchomego przeciwnika (czerwony kwadrat)
    tutorialEnemy.shape.setSize(sf::Vector2f(80.f, 80.f));
    tutorialEnemy.shape.setFillColor(sf::Color::Red);
    tutorialEnemy.shape.setOrigin(sf::Vector2f(40.f, 40.f));
    // Pozycja na ekranie: środek X, góra ekranu
    tutorialEnemy.fixedPositionOnBackground = sf::Vector2f(960.f, 200.f);
    tutorialEnemy.shape.setPosition(tutorialEnemy.fixedPositionOnBackground);
    tutorialEnemy.isActive = true;

    // Strzałka i tekst wskazujący na przeciwnika
    if (enemyArrowTexture.loadFromFile("Assets/Media/tutorial/arrow.png")) {
        enemyArrowSprite.emplace(enemyArrowTexture);
        enemyArrowSprite->setPosition(sf::Vector2f(930.f, 370.f));
        enemyArrowSprite->setScale(sf::Vector2f(0.5f, 0.5f));
        enemyArrowSprite->setRotation(sf::degrees(180.f));
        sf::Vector2u texSize = enemyArrowTexture.getSize();
        enemyArrowSprite->setOrigin(sf::Vector2f(texSize.x / 2.f, texSize.y / 2.f));
    }
    enemyInstructionText.emplace(font, "Wjedz w niego a dostaniesz obrazenia", 35);
    enemyInstructionText->setFillColor(sf::Color::Red);
    enemyInstructionText->setStyle(sf::Text::Bold);
    enemyInstructionText->setPosition(sf::Vector2f(250.f, 470.f));

    // Obrazek Spacebar-03.png
    if (spacebarTexture.loadFromFile("Assets/Media/tutorial/Spacebar-03.png")) {
        spacebarSprite.emplace(spacebarTexture);
        spacebarSprite->setPosition(sf::Vector2f(170.f, 780.f));
        spacebarSprite->setScale(sf::Vector2f(0.5f, 0.5f));
        sf::Vector2u texSize4 = spacebarTexture.getSize();
        spacebarSprite->setOrigin(sf::Vector2f(texSize4.x / 2.f, texSize4.y / 2.f));
    }
    if (spacebarArrowTexture.loadFromFile("Assets/Media/tutorial/arrow.png")) {
        spacebarArrowSprite.emplace(spacebarArrowTexture);
        spacebarArrowSprite->setPosition(sf::Vector2f(185.f, 920.f));
        spacebarArrowSprite->setScale(sf::Vector2f(-0.4f, 0.4f)); // Ujemne X = flip horizontal, dodatnie Y = normal
        spacebarArrowSprite->setRotation(sf::degrees(180.f));
        sf::Vector2u texSize5 = spacebarArrowTexture.getSize();
        spacebarArrowSprite->setOrigin(sf::Vector2f(texSize5.x / 2.f, texSize5.y / 2.f));
    }

    spacebarText.emplace(font, "Przytrzymaj spacje by strzelac", 35);
    spacebarText->setFillColor(sf::Color::Cyan);
    spacebarText->setStyle(sf::Text::Bold);
    spacebarText->setPosition(sf::Vector2f(260.f, 990.f));

    // Inicjalizacja UI
    if (!font.openFromFile("Assets/Fonts/BerlinSans.ttf")) {
        // handle error
    }

    scrapText.emplace(font);
    scrapText->setCharacterSize(30);
    scrapText->setFillColor(sf::Color::Yellow);
    scrapText->setPosition(sf::Vector2f(10.f, 10.f));
    scrapText->setString("Zlom: 0");

    // Inicjalizacja okrągłego paska HP
    hpCircle.setRadius(70.f);
    hpCircle.setPointCount(100);
    hpCircle.setFillColor(sf::Color::Transparent);
    hpCircle.setOutlineThickness(12.f);
    hpCircle.setOutlineColor(sf::Color::Green);
    hpCircle.setOrigin(sf::Vector2f(70.f, 70.f));

    hpInnerCircle.setRadius(58.f);
    hpInnerCircle.setPointCount(100);
    hpInnerCircle.setFillColor(sf::Color(101, 180, 179));
    hpInnerCircle.setOrigin(sf::Vector2f(58.f, 58.f));

    hpText.emplace(font, "10/10", 20);
    hpText->setFillColor(sf::Color::White);
    hpText->setPosition(sf::Vector2f(75.f, 170.f));

    healthText.emplace(font, "Zdrowie", 25);
    healthText->setFillColor(sf::Color::White);
    healthText->setStyle(sf::Text::Bold);



    // Obrazek strzałek - na prawej stronie ekranu
    if (arrowsTexture.loadFromFile("Assets/Media/tutorial/strzalki.png")) {
        arrowsSprite.emplace(arrowsTexture);
        arrowsSprite->setPosition(sf::Vector2f(1600.f, 650.f));
        arrowsSprite->setScale(sf::Vector2f(1.0f, 1.0f));
    }

    // Strzałka 1 - wskazuje na "Zlom"`
    if (arrowTexture1.loadFromFile("Assets/Media/tutorial/arrow.png")) {
        arrowSprite1.emplace(arrowTexture1);
        arrowSprite1->setPosition(sf::Vector2f(250.f, 50.f));
        arrowSprite1->setScale(sf::Vector2f(-0.4f, -0.4f));
        arrowSprite1->setRotation(sf::degrees(-90.f));
        sf::Vector2u texSize = arrowTexture1.getSize();
        arrowSprite1->setOrigin(sf::Vector2f(texSize.x / 2.f, texSize.y / 2.f));
    }
    arrowText1.emplace(font, "Zlom to waluta w tej grze, zbieraj go zabijajac przeciwnikow", 30);
    arrowText1->setFillColor(sf::Color::Yellow);
    arrowText1->setPosition(sf::Vector2f(325.f, 120.f));

    // Strzałka 2 -
    if (arrowTexture2.loadFromFile("Assets/Media/tutorial/arrow.png")) {
        arrowSprite2.emplace(arrowTexture2);
        arrowSprite2->setPosition(sf::Vector2f(1700.f, 920.f));
        arrowSprite2->setScale(sf::Vector2f(0.4f, 0.4f));
        arrowSprite2->setRotation(sf::degrees(180.f));
        sf::Vector2u texSize2 = arrowTexture2.getSize();
        arrowSprite2->setOrigin(sf::Vector2f(texSize2.x / 2.f, texSize2.y / 2.f));
    }
    arrowText2.emplace(font, "Uzyj strzalek do poruszania sie", 30);
    arrowText2->setFillColor(sf::Color::Cyan);
    arrowText2->setPosition(sf::Vector2f(1200.f, 1000.f));

    // Strzałka 3 - odbita lustrzanie w pionie
    if (arrowTexture3.loadFromFile("Assets/Media/tutorial/arrow.png")) {
        arrowSprite3.emplace(arrowTexture3);
        arrowSprite3->setPosition(sf::Vector2f(112.f, 305.f));
        arrowSprite3->setScale(sf::Vector2f(0.3f, -0.3f));
        sf::Vector2u texSize3 = arrowTexture3.getSize();
        arrowSprite3->setOrigin(sf::Vector2f(texSize3.x / 2.f, texSize3.y / 2.f));
    }
    arrowText3.emplace(font, "To jest twoje zdrowie", 30);
    arrowText3->setFillColor(sf::Color::Yellow);
    arrowText3->setPosition(sf::Vector2f(182.f, 350.f));

    // Inicjalizacja dwóch przeciwników do zestrzelenia (nieaktywni na początku)
    shootableEnemy1.shape.setSize(sf::Vector2f(60.f, 60.f));
    shootableEnemy1.shape.setFillColor(sf::Color::Blue);
    shootableEnemy1.position = sf::Vector2f(600.f, 300.f);
    shootableEnemy1.shape.setPosition(shootableEnemy1.position);
    shootableEnemy1.hp = 3;
    shootableEnemy1.isActive = false; // Nieaktywny na początku

    shootableEnemy2.shape.setSize(sf::Vector2f(60.f, 60.f));
    shootableEnemy2.shape.setFillColor(sf::Color::Magenta);
    shootableEnemy2.position = sf::Vector2f(1300.f, 300.f);
    shootableEnemy2.shape.setPosition(shootableEnemy2.position);
    shootableEnemy2.hp = 3;
    shootableEnemy2.isActive = false; // Nieaktywny na początku

    // Inicjalizacja paska postępu (pomarańczowy)
    progressBarBackground.setSize(sf::Vector2f(400.f, 30.f));
    progressBarBackground.setFillColor(sf::Color(50, 50, 50)); // Ciemny szary
    progressBarBackground.setPosition(sf::Vector2f(760.f, 50.f)); // Góra środek ekranu
    progressBarBackground.setOutlineThickness(3.f);
    progressBarBackground.setOutlineColor(sf::Color::White);

    progressBarFill.setSize(sf::Vector2f(0.f, 30.f)); // Zaczyna pusty
    progressBarFill.setFillColor(sf::Color(255, 140, 0)); // Pomarańczowy
    progressBarFill.setPosition(sf::Vector2f(760.f, 50.f));

    progressText.emplace(font, "Postep: 0/2", 25);
    progressText->setFillColor(sf::Color::White);
    progressText->setStyle(sf::Text::Bold);
    progressText->setPosition(sf::Vector2f(820.f, 90.f));

    // Inicjalizacja napisu "Wygrałeś"
    victoryText.emplace(font, "Wygra" "\xC5\x82" "es!", 80); // "Wygrałeś" z polskim ł
    victoryText->setFillColor(sf::Color::Green);
    victoryText->setStyle(sf::Text::Bold);
    sf::FloatRect victoryBounds = victoryText->getLocalBounds();
    victoryText->setOrigin(sf::Vector2f(victoryBounds.size.x / 2.f, victoryBounds.size.y / 2.f));
    victoryText->setPosition(sf::Vector2f(960.f, 480.f)); // Trochę wyżej

    // Inicjalizacja napisu "Naciśnij Enter"
    pressEnterText.emplace(font, "Nacisnij Enter aby kontynuowac", 40);
    pressEnterText->setFillColor(sf::Color::White);
    pressEnterText->setStyle(sf::Text::Bold);
    sf::FloatRect enterBounds = pressEnterText->getLocalBounds();
    pressEnterText->setOrigin(sf::Vector2f(enterBounds.size.x / 2.f, enterBounds.size.y / 2.f));
    pressEnterText->setPosition(sf::Vector2f(960.f, 600.f)); // Pod "Wygrałeś"

    // Inicjalizacja przyciemnienia ekranu po wygranej
    darkenOverlay.setSize(sf::Vector2f(1920.f, 1080.f));
    darkenOverlay.setFillColor(sf::Color(0, 0, 0, 150)); // Czarny z przezroczystością
    darkenOverlay.setPosition(sf::Vector2f(0.f, 0.f));
}

void Tutorial::handleEvent(const sf::Event& event, int* currentScreen) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        using Key = sf::Keyboard::Key;

        // Jeśli poziom ukończony, tylko ENTER działa (zapisuje i wraca do mapy)
        if (levelCompleted && key->code == Key::Enter) {
            // Zapisz ukończenie poziomu do save.json
            saveData.tutorialCompleted = true;
            // Odblokuj lvl 1 po ukończeniu tutorialu
            if (saveData.maxLevelUnlocked < 1) {
                saveData.maxLevelUnlocked = 1;
            }
            std::string saveFile = getSaveFileName();
            saveData.saveToFile(saveFile);

            // Wróć do mapy
            *currentScreen = 3;
            return;
        }

        // Jeśli gra zakończona, ignoruj inne klawisze
        if (levelCompleted) {
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
            *currentScreen = 3; // Powrót do mapy
        }
    }

    if (const auto* key = event.getIf<sf::Event::KeyReleased>()) {
        using Key = sf::Keyboard::Key;

        // Jeśli gra zakończona, ignoruj zwolnienia klawiszy
        if (levelCompleted) {
            return;
        }

        if (key->code == Key::Left || key->code == Key::Right) {
            velocity.x = 0.f;
        }
        if (key->code == Key::Up || key->code == Key::Down) {
            velocity.y = 0.f;
        }
    }
}

void Tutorial::update(float deltaTime, sf::Vector2u windowSize) {
    // Jeśli poziom ukończony, zatrzymaj grę (nie aktualizuj ruchu)
    if (levelCompleted) {
        // Tylko aktualizuj teksty UI
        if (hpText.has_value()) {
            hpText->setString(std::to_string(currentHP) + "/" + std::to_string(maxHP));
        }
        if (scrapText.has_value()) {
            scrapText->setString("Zlom: " + std::to_string(saveData.scrap));
        }
        return; // Zatrzymaj dalszą aktualizację
    }

    // Deklaracja backgroundHeight na początku funkcji
    float backgroundHeight = background.getHeight();

    // Automatyczne strzelanie gdy SPACE jest trzymany (jak w Level1)
    static float shootCooldown = 0.f;
    shootCooldown -= deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && shootCooldown <= 0.f) {
        // Stwórz pocisk
        float playerVelX = velocity.x;
        TutorialBullet bullet;
        bullet.shape.setRadius(5.f);
        bullet.shape.setFillColor(sf::Color::Red);
        bullet.shape.setPosition(sf::Vector2f(ball.getPosition().x + ball.getRadius(), ball.getPosition().y));
        bullet.velocity = sf::Vector2f(playerVelX * 0.02f, -300.f);
        bullets.push_back(bullet);
        shootCooldown = 0.15f; // Cooldown 150ms między strzałami
    }

    // Ruch gracza normalnie (wszystkie kierunki: lewo/prawo/góra/dół)
    ball.move(velocity * deltaTime);

    // Ograniczenia pozycji kulki - nie może wyjść poza ekran
    sf::Vector2f ballPos = ball.getPosition();
    float ballRadius = ball.getRadius();

    // Ograniczenie lewo
    if (ballPos.x - ballRadius < -20.f) {
        ball.setPosition(sf::Vector2f(ballRadius - 20.f, ballPos.y));
        velocity.x = 0.f;
    }
    // Ograniczenie prawo
    if (ballPos.x + ballRadius > static_cast<float>(windowSize.x)) {
        ball.setPosition(sf::Vector2f(static_cast<float>(windowSize.x) - ballRadius, ballPos.y));
        velocity.x = 0.f;
    }
    // Ograniczenie góra
    if (ballPos.y - ballRadius < 0.f) {
        ball.setPosition(sf::Vector2f(ballPos.x, ballRadius));
        velocity.y = 0.f;
    }
    // Ograniczenie dół
    if (ballPos.y + ballRadius > static_cast<float>(windowSize.y)) {
        ball.setPosition(sf::Vector2f(ballPos.x, static_cast<float>(windowSize.y) - ballRadius));
        velocity.y = 0.f;
    }

    // Ruch pocisków
    for (auto& bullet : bullets) {
        bullet.shape.move(bullet.velocity * deltaTime);
    }

    // Odbijanie pocisków od lewej i prawej ściany
    for (auto& bullet : bullets) {
        sf::Vector2f pos = bullet.shape.getPosition();
        float radius = bullet.shape.getRadius();

        if (pos.x - radius < 0.f || pos.x + radius > static_cast<float>(windowSize.x)) {
            bullet.velocity.x = -bullet.velocity.x;
        }
    }

    // Usuwanie pocisków poza ekranem (tylko góra)
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const TutorialBullet& b) {
        return b.shape.getPosition().y < 0;
    }), bullets.end());

    // Aktualizuj tekst HP
    if (hpText.has_value()) {
        hpText->setString(std::to_string(currentHP) + "/" + std::to_string(maxHP));
    }

    // Aktualizuj tekst złomu
    if (scrapText.has_value()) {
        scrapText->setString("Zlom: " + std::to_string(saveData.scrap));
    }

    // Aktualizuj pozycję nieruchomego przeciwnika - pozostaje na stałej pozycji
    if (tutorialEnemy.isActive) {
        // Przeciwnik pozostaje na swojej stałej pozycji - bez offsetY
        tutorialEnemy.shape.setPosition(tutorialEnemy.fixedPositionOnBackground);

        // Sprawdź kolizję gracza z nieruchomym przeciwnikiem
        sf::FloatRect playerBounds = ball.getGlobalBounds();
        sf::FloatRect enemyBounds = tutorialEnemy.shape.getGlobalBounds();

        if (playerBounds.findIntersection(enemyBounds)) {
            // Gracz wjechał w przeciwnika - zadaj obrażenia i ukryj przeciwnika
            currentHP--;
            if (currentHP < 0) currentHP = 0;

            // Zaktualizuj kolor paska HP
            if (currentHP > 6) {
                hpCircle.setOutlineColor(sf::Color::Green);
            } else if (currentHP > 3) {
                hpCircle.setOutlineColor(sf::Color::Yellow);
            } else {
                hpCircle.setOutlineColor(sf::Color::Red);
            }

            // Ukryj pierwszego przeciwnika i przejdź do fazy 2
            tutorialEnemy.isActive = false;
            tutorialPhase = 2;

            // Aktywuj dwóch przeciwników do zestrzelenia
            shootableEnemy1.isActive = true;
            shootableEnemy1.hp = 3;
            shootableEnemy2.isActive = true;
            shootableEnemy2.hp = 3;

            // Odrzuć gracza lekko do tyłu (w dół)
            ball.move(sf::Vector2f(0.f, 50.f));
        }
    }

    // Sprawdź kolizje pocisków z przeciwnikami do zestrzelenia (faza 2)
    if (tutorialPhase == 2) {
        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();) {
            bool bulletHit = false;

            // Sprawdź kolizję z pierwszym przeciwnikiem
            if (shootableEnemy1.isActive) {
                sf::FloatRect bulletBounds = bulletIt->shape.getGlobalBounds();
                sf::FloatRect enemy1Bounds = shootableEnemy1.shape.getGlobalBounds();

                if (bulletBounds.findIntersection(enemy1Bounds)) {
                    shootableEnemy1.hp--;
                    if (shootableEnemy1.hp <= 0) {
                        shootableEnemy1.isActive = false;
                        saveData.scrap += 10; // Nagroda za zestrzelenie
                        enemiesKilled++; // Zwiększ licznik zabitych
                    }
                    bulletHit = true;
                }
            }

            // Sprawdź kolizję z drugim przeciwnikiem
            if (!bulletHit && shootableEnemy2.isActive) {
                sf::FloatRect bulletBounds = bulletIt->shape.getGlobalBounds();
                sf::FloatRect enemy2Bounds = shootableEnemy2.shape.getGlobalBounds();

                if (bulletBounds.findIntersection(enemy2Bounds)) {
                    shootableEnemy2.hp--;
                    if (shootableEnemy2.hp <= 0) {
                        shootableEnemy2.isActive = false;
                        saveData.scrap += 10; // Nagroda za zestrzelenie
                        enemiesKilled++; // Zwiększ licznik zabitych
                    }
                    bulletHit = true;
                }
            }

            // Usuń pocisk jeśli trafił
            if (bulletHit) {
                bulletIt = bullets.erase(bulletIt);
            } else {
                ++bulletIt;
            }
        }

        // Aktualizuj pasek postępu
        float progressPercentage = static_cast<float>(enemiesKilled) / static_cast<float>(totalEnemies);
        progressBarFill.setSize(sf::Vector2f(400.f * progressPercentage, 30.f));

        if (progressText.has_value()) {
            progressText->setString("Postep: " + std::to_string(enemiesKilled) + "/" + std::to_string(totalEnemies));
        }

        // Sprawdź czy ukończono poziom (wszyscy przeciwnicy zabici)
        if (enemiesKilled >= totalEnemies && !levelCompleted) {
            levelCompleted = true;
        }
    }

    // Wszystkie elementy UI pozostają na swoich stałych pozycjach - brak offsetY
}

void Tutorial::draw(sf::RenderWindow& window) {
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = background.texture.getSize();

    // Rysuj ciemno-niebieski gradient jako tło
    sf::VertexArray gradient(sf::PrimitiveType::TriangleStrip, 4);

    // Ciemny niebieski na górze (RGB: 10, 20, 40)
    sf::Color topColor(10, 20, 40);
    // Bardzo ciemny niebieski na dole (RGB: 5, 10, 25)
    sf::Color bottomColor(5, 10, 25);

    // Lewy górny róg
    gradient[0].position = sf::Vector2f(0.f, 0.f);
    gradient[0].color = topColor;

    // Prawy górny róg
    gradient[1].position = sf::Vector2f(static_cast<float>(windowSize.x), 0.f);
    gradient[1].color = topColor;

    // Lewy dolny róg
    gradient[2].position = sf::Vector2f(0.f, static_cast<float>(windowSize.y));
    gradient[2].color = bottomColor;

    // Prawy dolny róg
    gradient[3].position = sf::Vector2f(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
    gradient[3].color = bottomColor;

    window.draw(gradient);

    // Rysuj gracza
    window.draw(ball);

    // FAZA 1: Rysuj pierwszego przeciwnika i wszystkie instrukcje
    if (tutorialPhase == 1 && tutorialEnemy.isActive) {
        window.draw(tutorialEnemy.shape);

        // Rysuj strzałkę i tekst wskazujący na przeciwnika
        if (enemyArrowSprite.has_value()) {
            window.draw(*enemyArrowSprite);
        }
        if (enemyInstructionText.has_value()) {
            window.draw(*enemyInstructionText);
        }

        // Rysuj obrazek Spacebar, strzałkę i tekst
        if (spacebarSprite.has_value()) {
            window.draw(*spacebarSprite);
        }
        if (spacebarArrowSprite.has_value()) {
            window.draw(*spacebarArrowSprite);
        }
        if (spacebarText.has_value()) {
            window.draw(*spacebarText);
        }

        // Rysuj instrukcje tutorialu
        if (instructionText.has_value()) {
            window.draw(*instructionText);
        }

        if (arrowsSprite.has_value()) {
            window.draw(*arrowsSprite);
        }

        // Rysuj 3 strzałki arrow.png z tekstami
        if (arrowSprite1.has_value()) {
            window.draw(*arrowSprite1);
        }
        if (arrowText1.has_value()) {
            window.draw(*arrowText1);
        }

        if (arrowSprite2.has_value()) {
            window.draw(*arrowSprite2);
        }
        if (arrowText2.has_value()) {
            window.draw(*arrowText2);
        }

        if (arrowSprite3.has_value()) {
            window.draw(*arrowSprite3);
        }
        if (arrowText3.has_value()) {
            window.draw(*arrowText3);
        }
    }

    // FAZA 2: Rysuj tylko dwóch przeciwników do zestrzelenia (bez napisów)
    if (tutorialPhase == 2) {
        if (shootableEnemy1.isActive) {
            window.draw(shootableEnemy1.shape);
        }
        if (shootableEnemy2.isActive) {
            window.draw(shootableEnemy2.shape);
        }

        // Rysuj pasek postępu (tylko w fazie 2)
        window.draw(progressBarBackground);
        window.draw(progressBarFill);
        if (progressText.has_value()) {
            window.draw(*progressText);
        }

        // Rysuj przyciemnienie i napis "Wygrałeś" jeśli poziom ukończony
        if (levelCompleted) {
            window.draw(darkenOverlay); // Przyciemnienie
            if (victoryText.has_value()) {
                window.draw(*victoryText); // Napis "Wygrałeś"
            }
            if (pressEnterText.has_value()) {
                window.draw(*pressEnterText); // Napis "Naciśnij Enter"
            }
        }
    }

    // Rysuj pociski (zawsze)
    for (const auto& bullet : bullets) {
        window.draw(bullet.shape);
    }

    // Rysuj UI ze złomem
    if (scrapText.has_value()) {
        window.draw(*scrapText);
    }

    // Rysuj okrągły pasek HP
    sf::Vector2f hpPosition(100.f, 150.f);

    hpInnerCircle.setPosition(hpPosition);
    window.draw(hpInnerCircle);

    sf::CircleShape hpBackground;
    hpBackground.setRadius(70.f);
    hpBackground.setPointCount(100);
    hpBackground.setFillColor(sf::Color::Transparent);
    hpBackground.setOutlineThickness(12.f);
    hpBackground.setOutlineColor(sf::Color(80, 80, 80, 150));
    hpBackground.setOrigin(sf::Vector2f(70.f, 70.f));
    hpBackground.setPosition(hpPosition);
    window.draw(hpBackground);

    float hpPercentage = static_cast<float>(currentHP) / static_cast<float>(maxHP);
    const int segments = static_cast<int>(100 * hpPercentage);
    sf::VertexArray hpArc(sf::PrimitiveType::TriangleStrip);

    float radius = 70.f;
    float thickness = 12.f;
    float angleRange = 360.f * hpPercentage;

    sf::Color hpColor;
    if (currentHP > 6) {
        hpColor = sf::Color::Green;
    } else if (currentHP > 3) {
        hpColor = sf::Color::Yellow;
    } else {
        hpColor = sf::Color::Red;
    }

    for (int i = 0; i <= segments; ++i) {
        float angle = (270.f - angleRange / 2.f + (angleRange * static_cast<float>(i) / 100.f)) * 3.14159f / 180.f;

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

    // Rysuj tekst "Zdrowie"
    if (healthText.has_value()) {
        sf::FloatRect textBounds = healthText->getLocalBounds();
        healthText->setOrigin(sf::Vector2f(textBounds.size.x / 2.f, textBounds.size.y / 2.f));
        healthText->setPosition(sf::Vector2f(hpPosition.x, hpPosition.y - 25.f));
        window.draw(*healthText);
    }

    if (hpText.has_value()) {
        window.draw(*hpText);
    }
}

void Tutorial::setSaveSlot(int slot) {
    currentSaveSlot = slot;
    std::string saveFile = getSaveFileName();
    if (SaveData::fileExists(saveFile)) {
        saveData.loadFromFile(saveFile);
    }
}

void Tutorial::reset() {
    // Reset pozycji gracza - środek ekranu
    ball.setPosition(sf::Vector2f(960.f, 540.f));
    velocity = sf::Vector2f(0.f, 0.f);

    // Wyczyść pociski
    bullets.clear();

    // Reset HP
    currentHP = maxHP;
    hpCircle.setOutlineColor(sf::Color::Green);

    // Reset przeciwnika
    tutorialEnemy.isActive = true;


    if (hpText.has_value()) {
        hpText->setString(std::to_string(currentHP) + "/" + std::to_string(maxHP));
    }
}

std::string Tutorial::getSaveFileName() const {
    return "Assets/save" + std::to_string(currentSaveSlot) + ".json";
}
