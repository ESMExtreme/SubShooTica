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

    // Ustaw offset tła tak aby pokazywał DÓŁ tła (gracz zaczyna na dole)
    // Zakładając wysokość okna 1080, ustaw offset na wysokość_tła - 1080
    float backgroundHeight = static_cast<float>(background.texture.getSize().y);
    background.offsetY = std::max(0.f, backgroundHeight - 1080.f);

    // Inicjalizacja piłki z teksturą seam.png
    ball.setRadius(70.f);
    ball.setFillColor(sf::Color::White);
    // Gracz zaczyna na dole ekranu (y=900 to prawie dół przy rozdzielczości 1080p)
    ball.setPosition(sf::Vector2f(960.f, 950.f)); // Środek ekranu w poziomie, blisko dołu w pionie

    if (playerTexture.loadFromFile("Assets/Media/seam.png")) {
        ball.setTexture(&playerTexture);
    }

    velocity = sf::Vector2f(0.f, 0.f);
    bulletVelocity = sf::Vector2f(0.f, -300.f);

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

    // Instrukcje tutorialu - przypięte do tła (na dole tła, gdzie gracz zaczyna)
    instructionText.emplace(font);
    instructionText->setCharacterSize(40);
    instructionText->setFillColor(sf::Color::White);
    instructionText->setStyle(sf::Text::Bold);
    instructionText->setString("TUTORIAL\n\nUzyj strzalek do poruszania\nSpacja - strzelanie\nESC - powrot do mapy");
    // Pozycja względem wysokości tła - umieść instrukcje na dole tła gdzie gracz zaczyna
    // Używamy zmiennej backgroundHeight która już istnieje (linia 16)
    instructionText->setPosition(sf::Vector2f(600.f, backgroundHeight - 780.f)); // 780 od dołu tła

    // Załaduj obrazek strzałek - również na dole tła
    if (arrowsTexture.loadFromFile("Assets/Media/tutorial/strzalki.png")) {
        arrowsSprite.emplace(arrowsTexture);
        arrowsSprite->setPosition(sf::Vector2f(650.f, backgroundHeight - 580.f)); // 580 od dołu tła
        arrowsSprite->setScale(sf::Vector2f(1.0f, 1.0f)); // Zwiększona skala (było 0.5)
    }
}

void Tutorial::handleEvent(const sf::Event& event, int* currentScreen) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        using Key = sf::Keyboard::Key;

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
        if (key->code == Key::Space) {
            // Strzelanie
            TutorialBullet bullet;
            bullet.shape.setRadius(10.f);
            bullet.shape.setFillColor(sf::Color::Yellow);
            bullet.shape.setPosition(ball.getPosition());
            bullet.velocity = bulletVelocity;
            bullets.push_back(bullet);
        }
        if (key->code == Key::Escape) {
            *currentScreen = 3; // Powrót do mapy
        }
    }

    if (const auto* key = event.getIf<sf::Event::KeyReleased>()) {
        using Key = sf::Keyboard::Key;

        if (key->code == Key::Left || key->code == Key::Right) {
            velocity.x = 0.f;
        }
        if (key->code == Key::Up || key->code == Key::Down) {
            velocity.y = 0.f;
        }
    }
}

void Tutorial::update(float deltaTime, sf::Vector2u windowSize) {
    // Ruch gracza
    ball.move(velocity * deltaTime);

    // Ograniczenia pozycji kulki
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

    // Tło porusza się synchronicznie z graczem (taka sama prędkość)
    // Przesuń tło wraz z ruchem gracza w pionie
    if (velocity.y != 0.f) {
        background.offsetY -= velocity.y * deltaTime; // Minus, bo gdy gracz idzie w górę (ujemny velocity.y), offset maleje

        // Ogranicz scrollowanie tła
        if (background.offsetY < 0.f) {
            background.offsetY = 0.f;
        }

        float maxOffset = background.getHeight() - static_cast<float>(windowSize.y);
        if (background.offsetY > maxOffset) {
            background.offsetY = maxOffset;
        }
    }

    // Ograniczenie góra - nie pozwól graczowi wyjść poza górę ekranu gdy tło osiągnie początek
    if (ballPos.y - ballRadius < 0.f && background.offsetY <= 0.f) {
        ball.setPosition(sf::Vector2f(ballPos.x, ballRadius));
        velocity.y = 0.f;
    }

    // Ograniczenie dół - nie pozwól graczowi wyjść poza dół ekranu gdy tło osiągnie koniec
    float maxBackgroundOffset = background.getHeight() - static_cast<float>(windowSize.y);
    if (ballPos.y + ballRadius > static_cast<float>(windowSize.y) && background.offsetY >= maxBackgroundOffset) {
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
}

void Tutorial::draw(sf::RenderWindow& window) {
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = background.texture.getSize();

    // Rysuj tło (nie scrollowane automatycznie, tylko przesunięte wraz z graczem)
    if (background.sprite.has_value() && textureSize.x > 0 && textureSize.y > 0) {
        float texW = static_cast<float>(background.texture.getSize().x);
        float texH = static_cast<float>(background.texture.getSize().y);
        float scaleX = static_cast<float>(windowSize.x) / texW;
        float scaleY = static_cast<float>(windowSize.y) / texH;

        // Użyj większej skali aby wypełnić ekran
        float scale = std::max(scaleX, scaleY);

        background.sprite->setScale(sf::Vector2f(scale, scale));
        // Przesuń tło zgodnie z offsetem (camera follow)
        background.sprite->setPosition(sf::Vector2f(0.f, -background.offsetY * scale));
        window.draw(*background.sprite);
    }

    // Rysuj gracza
    window.draw(ball);

    // Rysuj pociski
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

    // Rysuj instrukcje tutorialu - przypięte do tła, nie do kamery
    // Przesuwamy je wraz z tłem używając background.offsetY
    if (instructionText.has_value()) {
        float backgroundHeight = static_cast<float>(background.texture.getSize().y);
        // Oblicz pozycję względem tła
        float relativeY = backgroundHeight - 780.f;
        // Przesuń pozycję o offset tła (aby poruszało się z tłem)
        instructionText->setPosition(sf::Vector2f(600.f, relativeY - background.offsetY));
        window.draw(*instructionText);
    }

    if (arrowsSprite.has_value()) {
        float backgroundHeight = static_cast<float>(background.texture.getSize().y);
        float relativeY = backgroundHeight - 580.f;
        arrowsSprite->setPosition(sf::Vector2f(650.f, relativeY - background.offsetY));
        window.draw(*arrowsSprite);
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
    // Reset pozycji gracza - na dole ekranu
    ball.setPosition(sf::Vector2f(960.f, 950.f));
    velocity = sf::Vector2f(0.f, 0.f);

    // Wyczyść pociski
    bullets.clear();

    // Reset HP
    currentHP = maxHP;
    hpCircle.setOutlineColor(sf::Color::Green);

    // Reset tła - zaczynamy od DOŁU tła (maksymalny offset)
    float backgroundHeight = background.getHeight();
    background.offsetY = std::max(0.f, backgroundHeight - 1080.f);

    if (hpText.has_value()) {
        hpText->setString(std::to_string(currentHP) + "/" + std::to_string(maxHP));
    }
}

std::string Tutorial::getSaveFileName() const {
    return "Assets/save" + std::to_string(currentSaveSlot) + ".json";
}
